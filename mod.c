#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void sig_handler(int sig)
{
	signal(SIGINT, sig_handler);
	printf("\n");
	printf("xyz> ");
	fflush(stdout);
}

void change_dir(char ** args)
{
	if(args[1]==NULL)
	{
		chdir("/\0");
		chdir("/home/lirus\0");
	}
	else
	{
		if(chdir(args[1])==-1)
		{
			perror("");
		}
	}
}

char * read_input()
{
	ssize_t size = 0;
	char * line = NULL;
	getline(&line, &size, stdin);
	return line;
}

char ** parser(char *line)
{
	int size=64, i=0;
	char **args = malloc(size*sizeof(char *));
	char *token;
	if(!args)
	{
		perror("allocation error");
		exit(EXIT_FAILURE);
	}
	int l1, l2;
	l1=l2=0;
	for(int i=0;i<strlen(line);i++)
		if(line[i]=="<" || line[i]==">")
		{
			l2=i-1;
			int j=0;
			char * temp;
			for(j=0;j<l2;j++)
				temp[j]=line[j];
			temp[j]=NULL;
		}
	token=strtok(line, " \t\r\n\a");
	while(token!=NULL)
	{
		args[i++]=token;
		if(i>=size)
		{
			args=realloc(args, size*sizeof(char *));
			if(!args)
			{
				perror("allocation error");
				exit(EXIT_FAILURE);
			}
		}
		token=strtok(NULL, " \t\r\n\a");
	}
	args[i]=NULL;
	return args;
}

char *** parse_input(char *line)
{
	char *** args = malloc(200*1024*sizeof(char *));
	char ** pipes = malloc(1024*sizeof(char *));
	char *token;
	int k=0;
	token=strtok(line, "|");
	while(token!=NULL)
	{
		pipes[k++]=token;
		token=strtok(NULL, "|");
	}
	pipes[k]=NULL;
	int i;
	for(i=0;i<k;i++)
	{
		args[i]=parser(pipes[i]);
	}
	args[i]=NULL;
	return args;
}

int main()
{
	char history[1111][111];
	int k=0;
	signal(SIGINT, sig_handler);
	while (1) {
		printf("xyz> ");
		int flag = 0;
		char * line;
		line = read_input();
		strcpy(history[k++], line);
		int f = 0;
		for (int i = 0; i < strlen(line); ++i)
			if (line[i] != ' ')
				f = 1;
		if (!f) continue; if (!strlen(line)) flag = 1;
		if (!flag)
		{
			char *** args = parse_input(line);
			int j=0;
			if (!strcmp(**args, "exit"))
				exit(0);
			else if(!strcmp(**args, "cd"))
				change_dir(*args);
			else if(!strcmp(**args, "history"))
			{
				for(int i=0;i<k;i++)
					printf("%s", history[i]);
			}
			else
			{
				int j=0;
				int fd[2];
				int fdp=0;
				while(args[j]!=NULL)
				{
					pipe(fd);
					pid_t id=fork();
					int status=0;
					if (id == 0)
					{
						dup2(fdp, 0);
						if(args[j+1]!=NULL)
							dup2(fd[1], 1);
						close(fd[0]);
						printf("%s1%s1\n",args[0][0],args[1][0]);
						execvp(args[j][0], args[j]);
						perror("Execution error\n");
					}
					else
					{
						waitpid(id, &status, 0);
						close(fd[1]);
						fdp=fd[0];
					}
					j++;
				}
			}
			free(line);
		}
	}
}
