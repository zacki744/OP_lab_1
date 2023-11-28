#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define INT_MAX 2147483647
#define PERMS 0644
struct my_msgbuf {
   long mtype;
   int mtext;
};

int main(void) {
   struct my_msgbuf buf;
   int msqid;
   //int len;
   key_t key;
   int msg_end = -1;
   system("touch msgq.txt");

   if ((key = ftok("msgq.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }

   if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue: ready to send messages.\n");

/*the old code
   printf("Enter lines of text, ^D to quit:\n");
   buf.mtype = 1; /* we don't really care in this case *

   while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {
      len = strlen(buf.mtext);
      /* remove newline at end, if it exists *
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' *
         perror("msgsnd");
   }*/
   //the new
   // Generate and send a sequence of 50 integers
   for (int i = 0; i < 50; i++) {
      int random_int = rand() % INT_MAX;
      buf.mtype = 1;
      buf.mtext = random_int;
      if (msgsnd(msqid, &buf, sizeof(int), 0) == -1) /* +1 for '\0' */
         perror("msgsnd");
   }
   /*
   strcpy(buf.mtext, "end");
   len = strlen(buf.mtext);
   if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' *
      perror("msgsnd");

   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
      exit(1);
   }
   printf("message queue: done sending messages.\n");
   return 0;*/
   buf.mtext = msg_end;
   if (msgsnd(msqid, &buf, sizeof(int), 0) == -1) /* +1 for '\0' */
      perror("msgsnd");

   if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue: done sending messages.\n");
   return 0;
}
