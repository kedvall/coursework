
#ifndef __28335_EQEP_H__
#define __28335_EQEP_H__


#define QEP_MAXVALUE		ULONG_MAX

typedef struct {
	enum eQep_e eq;
	int32 cpr;
	int pol;
	float res;
	float init_rad_coord;
} eqep_t;

void init_EQEP(eqep_t *eqep, enum eQep_e eq, int32 cpr, int pol, float init_rad_coord);
float EQEP_read(eqep_t *eqep);

#endif /* __2808_EQEP_H__ */


