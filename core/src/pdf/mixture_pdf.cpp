#include "mixture_pdf.h"

#include "../misc/singleton.h"


double mixture_pdf::value(const vector3& direction) const
{
	return proportion * (p[0]->value(direction)) + (1.0 - proportion) * (p[1]->value(direction));
}

vector3 mixture_pdf::generate(scatter_record& rec)
{
	if (Singleton::getInstance()->rnd().get_real(0.0, 1.0) < proportion)
	{
		return p[0]->generate(rec);
	}
	else
	{
		return p[1]->generate(rec);
	}
}