#ifndef _IR_RECEIVER_H_
#define _IR_RECEIVER_H_

void ir_init(void);

extern volatile unsigned char ir_last_command;

#define IR_COMMAND_NONE     0
#define IR_COMMAND_PREVIOUS 1
#define IR_COMMAND_NEXT     2

#endif /* _IR_RECEIVER_H_ */
