#include "Types.h"

/* Prints register to the output. */
std::ostream& operator<<(std::ostream& out, Regs r)
{
	switch (r)
	{
	case t0: out << "t0"; break;
	case t1: out << "t1"; break;
	case t2: out << "t2"; break;
	case t3: out << "t3"; break;
	case t4: out << "t4"; break;
	case t5: out << "t5"; break;
	case t6: out << "t6"; break;
	}
	return out;
}