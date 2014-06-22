#ifndef OPCAJA_H
#define OPCAJA_H

const long MSG_CONSULTAR_CAJA = 1;
const long MSG_ACREDITAR_MONTO = 2;

struct OpCaja
{
	// Tipo de mensaje que se transmitira al proceso de Caja.
	long mtype;
	// Tipo de respuesta (pid del proceso Empleado o Administrador)
	long rtype;
	// Monto a acreditar
	float monto;
};

#endif /* OPCAJA_H */
