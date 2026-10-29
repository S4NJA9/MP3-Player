#ifndef TYPE_H
#define TYPE_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_failure,
    e_success
} Status;

typedef enum
{
    v_view,
    e_edit,
    u_unsupported
} OperationType;

#endif