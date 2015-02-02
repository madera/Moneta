# Codecs

## Decoding

Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.

### Simple trivial decode

#### Ultra simple decoding
To read a single Entity in a serialized form, the exact same way that it was encoded (that is, the same serialization call order) you need nothing more than a simple decode_one() call:

	template <typename InputIterator, class Entity>
	int decode(InputIterator& next, InputIterator end, Entity& person);

The decode_one() function will attempt to decode the given entity and give you the Codec Return.

If you can guarantee that nobody will change the file (or stream) that you created with encode(), chances are you can get away with using this simplest method.

#### Decoding unknown entities
In a more realistical real world application you won't know exactly what entity you will find and in which order. Specially on files that are edited by users (such as text-based files). Consider an XML file, where the fields can appear in any order the editor wishes.

To decode such streams, we need either one of two things:

 * A list of Entities which are to be expected
 * A typecode_reader functor

##### Entity Group based Decoding

	template <class EntityList, typename RandomAccessIterator, class Visitor>
	int decode(RandomAccessIterator& next, RandomAccessIterator end, Visitor& visitor);

What this will do is try to decode the stream using all available decoders for the given Entities. That is, a Daisy call Chain will be used until a decoder can handle that codec.

The downside here is that InputIterator is no longer supported, and a RandomAccessIterator must be supplied. If this is a problem for you, then you need a typecode_reader. See below.

If you wish to decode as many entities as you can on the given range, you can use:
	
	template <class EntityList, typename RandomAccessIterator, class Visitor>
	int decode_many(RandomAccessIterator& next, RandomAccessIterator end, Visitor& visitor);

Which is basically the same as decode_one() except that it will loop until no decoder can provide an entity anymore.

##### Typecode Readers
To get the most out of your decoding, you should provide a typecode_reader functor.

When such a facility is given to the decoder, it will use it to read the minimum possible amount of data from the input stream. This will be just enough to pinpoint which entity is exactly coded, and the codec for that entity will be dispatched.

For instance, if you have an XML decoder, the typecode_reader will read "<Person" and immediately know we are talking about a Person entity instance. So the decoder will transfer control to the Person entity decoder for fast decoding.

	template <typename InputIterator, class Visitor>
	int decode_one(InputIterator& next, InputIterator end, Visitor& visitor);
	
Since a typecode_reader was provided, no entity list is needed.
