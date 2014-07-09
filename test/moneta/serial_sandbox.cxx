#include "stdafx.h"
#include <moneta/codec/rawbin.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include <moneta/pp/sql_entity.hxx> // XXX: XXX: XXX!!!
#include <moneta/codec/multi_decoder.hxx>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename U, class EntityType>
typename boost::enable_if<
	moneta::traits::is_entity<EntityType>,
	std::basic_ostream<T, U>
>::type&
operator<<(std::basic_ostream<T, U>& output, const EntityType& entity) {
	std::cout << typeid(EntityType).name() << std::endl;
	return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	ServerHandshake,
	((int, Magic   ))
	((int, Category))
	((int, Unknown ))
	((int, Version ))
)

MONETA_FIXED_VALUE(MONETA_MEMBER(ServerHandshake, int, Magic   ), 'vOeG')
MONETA_FIXED_VALUE(MONETA_MEMBER(ServerHandshake, int, Category), 0x0010)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	ClientHandshake,
	((int, Magic   ))
	((int, Category))
	((int, SomeVersion))
	((int, Unknown1))
)

MONETA_FIXED_VALUE(MONETA_MEMBER(ClientHandshake, int, Magic   ), 'VoEg')
MONETA_FIXED_VALUE(MONETA_MEMBER(ClientHandshake, int, Category), 0x0010)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace server {
	struct auth_challenge {
		int Magic;
		int Category;
		int SomeVersion;
		int Sequence;
		char Key[16];
		int Unknown1;
	};
}

namespace moneta { namespace traits { namespace detail {
	template <>
	struct members_of<server::auth_challenge> : boost::mpl::vector<
		MONETA_MEMBER(server::auth_challenge, int,      Magic      ),
		MONETA_MEMBER(server::auth_challenge, int,      Category   ),
		MONETA_MEMBER(server::auth_challenge, int,      SomeVersion),
		MONETA_MEMBER(server::auth_challenge, int,      Sequence   ),
		MONETA_MEMBER(server::auth_challenge, char[16], Key        ),
		MONETA_MEMBER(server::auth_challenge, int,      Unknown1   )
	> {};
}}}

MONETA_MEMBER_NAME(MONETA_MEMBER(server::auth_challenge, int,      Magic      ), Magic   )
MONETA_MEMBER_NAME(MONETA_MEMBER(server::auth_challenge, int,      Category   ), Category)
MONETA_MEMBER_NAME(MONETA_MEMBER(server::auth_challenge, int,      SomeVersion), SomeVersion)
MONETA_MEMBER_NAME(MONETA_MEMBER(server::auth_challenge, int,      Sequence   ), Sequence)
MONETA_MEMBER_NAME(MONETA_MEMBER(server::auth_challenge, char[16], Key        ), Key     )
MONETA_MEMBER_NAME(MONETA_MEMBER(server::auth_challenge, int,      Unknown1   ), Unknown1)

 MONETA_FIXED_VALUE(MONETA_MEMBER(server::auth_challenge, int, Magic   ), 'vOeG')
 MONETA_FIXED_VALUE(MONETA_MEMBER(server::auth_challenge, int, Category), 0x0024)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct non_entity {};

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

template <class Operation>
class client {
	typedef std::vector<unsigned char> bytes_t;

	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::socket _socket;

	bytes_t _read_buffer;

	Operation _operation;

public:
	client(const std::string host, const size_t port, boost::asio::io_service& io_service)
	 : _io_service(io_service), _socket(_io_service), _read_buffer(1024), _operation(*this) {
		using boost::asio::ip::tcp;
		tcp::resolver resolver(_io_service);
		tcp::resolver::query query(host, boost::lexical_cast<std::string>(port));
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end) {
			_socket.close();
			_socket.connect(*endpoint_iterator++, error);
		}

		if (error) {
			throw boost::system::system_error(error);
		}

		using namespace boost::asio;
		_socket.async_read_some(
			boost::asio::buffer(_read_buffer),
			boost::bind(&client::handle_read, this, placeholders::error, placeholders::bytes_transferred)
		);
	}

	template <class EntityType>
	void sync_send(EntityType& entity) {
		const unsigned char* data = reinterpret_cast<unsigned char*>(&entity);
		const size_t size = sizeof(EntityType);

		std::cout << ">>> " << typeid(EntityType).name() << std::endl;
		hexdump_line(data, size);

		boost::system::error_code ignored_error;
		boost::asio::write(
			_socket,
			boost::asio::buffer(data, size),
			boost::asio::transfer_all(),
			ignored_error
		);
	}

private:
	void handle_read(const boost::system::error_code& error, const std::size_t size) {
		if (error) {
			int x = 0;
		}
		
		if (size == 0) {
			int y = 0;
		}

		std::cout << "read " << size << " size: " << std::endl;
		hexdump_line(_read_buffer.data(), size);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		using moneta::codec::rawbin;
		typedef moneta::codec::multi_decoder<rawbin, ServerHandshake, server::auth_challenge> decoder_type;
		decoder_type::variant_type entity;
		decoder_type decoder;

		int result = decoder(entity, _read_buffer.data(), _read_buffer.data() + size);
		if (result > 0) {
			boost::apply_visitor(_operation, entity);
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		using namespace boost::asio;
		_socket.async_read_some(
			boost::asio::buffer(_read_buffer),
			boost::bind(&client::handle_read, this, placeholders::error, placeholders::bytes_transferred)
		);
	}
};

#include <moneta/serialization/shell/shell_codec.hxx>

class printer : public boost::static_visitor<void> {
	typedef client<printer> client_type;
	client_type& _client;
public:
	printer(client_type& client)
 	 : _client(client) {}

	void operator()(const ServerHandshake& handshake) const {
		std::cout << ">>> ServerHandshake" << std::endl;
		std::cout << moneta::serialization::shell::to_line(handshake) << std::endl;

		ClientHandshake response = moneta::make_entity<ClientHandshake>();
		response.SomeVersion = 3;
		_client.sync_send(response);
	}

	void operator()(const server::auth_challenge& challenge) const {
		std::cout << ">>> server::auth_challenge" << std::endl;
		std::cout << moneta::serialization::shell::to_line(challenge) << std::endl;

	}
};

BOOST_AUTO_TEST_CASE(serial_sandbox) {
	boost::asio::io_service io_service;
	client<printer> client("10.0.0.145", 10000, io_service);
	//io_service.run();
}
