map
You have to provide an ordering function (or functional object) for std::map. If not, it defaults to std::less, which in turn uses < by default.The map will be in ascending order according to the values of the key. Internally, the map performs a comparison between keys to order its elements. By default, it uses std::less>KEY>, which is equivalent to bool operator<(int, int) for integers. 
The fundamental property of iterators of associative containers is that they
iterate through the containers in the non-descending order of keys where
non-descending is defined by the comparison that was used to construct them.
For any two dereferenceable iterators i and j such that distance from i to j is
positive,  value_comp(*j, *i) == false
"<": The less-than operator on strings does a lexicographical comparison on the strings. This compares strings in the same way that they would be listed in dictionary order, generalized to work for strings with non-letter characters. E.G "20160106" < "20170320"