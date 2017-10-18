
#ifndef ARI_ITERATOR_BASE_H
#define ARI_ITERATOR_BASE_H

namespace ari {

template <
	typename TType,
	typename TDist  = ptrdiff_t, 
	typename TPoint = TType*, 
	typename TRef   = TType&
> struct iterator_base {
	using value_type      = TType;
	using difference_type = TDist;
	using pointer         = TPoint;
	using reference       = TRef;
};

} // end namespace ari

#endif