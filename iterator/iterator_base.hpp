
#ifndef ARI_ITERATOR_BASE_H
#define ARI_ITERATOR_BASE_H

#include <cstddef>

namespace ari {

template <
	typename TType,
	typename TDist  = std::ptrdiff_t, 
	typename TPoint = TType*, 
	typename TRef   = TType&
> struct iterator_base {
	using value_type      = TType;
	using difference_type = TDist;
	using pointer         = TPoint;
	using reference       = TRef;
};

} // end namespace ari

#endif // ARI_ITERATOR_BASE_H defined