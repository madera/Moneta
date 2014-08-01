## Codecs

### Internals: Inner traversal

	moneta::codec::encode<Codec>(entity, begin, end);

* moneta::codec::encode()
 * for each member: member_encoder<Codec, Member, Path>
   * encoder<Codec, typename Member::result_type>()(member(entity), begin, end, Path());
     (or without path).
	
