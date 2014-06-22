#ifndef OPJEFE_H
#define OPJEFE_H

const long MSG_EMPLEADO_LIBRE = 1;
const long MSG_AUTO_VIP = 2;
const long MSG_AUTO_REGULAR = 3;

struct OpJefe
{
	// Tipo de mensaje que se transmitira al jefe.
	long mtype;
	// Id del empleado devuelto al jefe.
	int idEmp;
	// Litros de combustible que va a cargar el auto enviado al jefe.
	int litros;
};

#endif /* OPJEFE_H */
