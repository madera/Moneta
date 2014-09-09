# Codecs

Moneta is capable of both Object-Relational Mapping (ORM) and Serialization. The implementation core that allows ORM is the same code that facilitates serialization, but only to a certain extent.

Moneta is all about providing information about your model domain. You annotate your classes so Moneta can do it's magic at compile time, giving your the performance only Template Metaprogramming can provide.

## Serialization

Serialization in Moneta is full os details, but its usage is very simple for the end user.

The first thing to know about Serialization in Moneta is that it's based on Codecs. ((Codec is a portmanteau of "coder-decoder")). In practice you define a type tag that will specialize an encoder and a decoder class on the moneta::codec namespace.

Some Codecs are readily available in Moneta for use, such as XML, Shell, and Rawbin.

XML needs no introduction. Shell is a simplified JSON syntax codec, and rawbin a very simple binary dump, to serve more as an example than anything else.

## Encoding

Encoding an entity is done using moneta::encode<>(), passing the Codec tag to use, the entity instance, and an output buffer range.

// EXAMPLE

## Codec Internals

Please read Codec Introduction first.


### Internals: Inner traversal

	moneta::codec::encode<Codec>(entity, begin, end);

* moneta::codec::encode()
 * for each member: member_encoder<Codec, Member, Path>
   * encoder<Codec, typename Member::result_type>()(member(entity), begin, end, Path());
     (or without path).
	



### Externals

#### Encoder

When moneta::encode<>() is called it creates a functor of entity_encoder<Codec> type. By default, this class begins an in-order traversal over the entity.

This is the default sequence of an encode<>() call:

* moneta::encode<Codec>(const Entity& entity, Iterator begin, Iterator end);
  * entity_encoder<Codec, Entity>    
    * enter_entity_encoder<Codec, Entity>
    * member_encoder<Codec, Entity>
      * value_encoder<Codec, T>.
    * leave_entity_encoder<Codec, E  T>.
ity>

You may specialize any of these classes at will. Just remember that the other calls won't be made and chaining will need to be continued manually if wanted.

#### Decoder

Just as the previous, moneta::decode<>() has a default set of implementation classes that will iterate over a given entity. However, this is not the best case implementation for several Codecs.


