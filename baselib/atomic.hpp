

#include "basedef.h"
#include "basefunction.h"
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

namespace ROOT_NS
{
namespace BASELIB_NS
{


template<typename T>
class basic_atom: boost::noncopyable
{
private:
	T n;
	boost::mutex mu;
public:
	basic_atom(T x = T()) :
			n(x)
	{
	}
	T operator++()
	{
		boost::mutex::scoped_lock lock(mu);
		return ++n;
	}
	operator T()
	{
		return n;
	}

	T operator +=(T x)
	{
		boost::mutex::scoped_lock lock(mu);
		return n = n + x;
	}
	T get_value()
	{
		return n;
	}
};

typedef basic_atom<int>	atomic_int;
typedef basic_atom<float> atomic_float;

}
}
