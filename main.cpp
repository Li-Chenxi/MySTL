#include "MySTL_type_traits.h"
#include "MySTL_iterator.h"
#include "MySTL_allocate.h"
#include "MySTL_construct.h"
#include "MySTL_vector.h"

int main()
{
	using std::cout;
	using std::endl;

	vector<int, allocator> v;
	for (int i = 0; i < 4; i++)
		v.push_back(i);

	for (int i = 0; i < 4; i++)
		cout << v[i] << endl;
	return 0;
}