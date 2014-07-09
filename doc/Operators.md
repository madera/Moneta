TODO:
	* Add support for pk references, such as sub_tie<data_Tuple, mpl::vector<0, 1, 5> >
	* Facilitate test entity creation. Right now, Garfield is all over the place!

Document and track development for these:

	moneta::tuple					<moneta/traits/tuple.hxx>
	moneta::tie					<moneta/traits/tie.hxx>
	
	moneta::to_tuple				<moneta/traits/to_tuple.hxx>
		to_tuple(const ET)				   "
	moneta::to_tie					<moneta/traits/to_tie.hxx>
		to_tie(const ET)				   "
	
 	moneta::pk					<moneta/traits/pk.hxx>
 	moneta::pk_tie					<moneta/traits/pk_tie.hxx>
	
 	moneta::db_tuple				<moneta/sql/traits/db_tuple.hxx>
 	moneta::db_tie					<moneta/sql/traits/db_tie.hxx>
		
	moneta::to_db_tuple				<moneta/sql/traits/to_db_tuple.hxx>
		to_db_tuple(const ET)				   "
	moneta::to_db_tie				<moneta/sql/traits/to_db_tie.hxx>
		to_db_tie(const ET)				   "

