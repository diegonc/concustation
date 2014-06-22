#ifndef OPSURTIDORES_H
#define OPSURTIDORES_H

const long MSG_DEVOLVER_SURTIDOR = 1;
const long MSG_SOLICITAR_SURTIDOR = 2;

struct OpSurtidores
{
	// Tipo de mensaje que se transmitira al proceso de Surtidores.
	long mtype;
	// Tipo de respuesta (pid del proceso Empleado)
	long rtype;
	// ID de surtidor
	int idSurtidor;
};

#endif /* OPSURTIDORES_H */
