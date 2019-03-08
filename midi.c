#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/soundcard.h>
#include <pthread.h>
#include <stdlib.h>

#define MIDI_DEVICE "/dev/midi2"
#define SPEAKER_OUT "/dev/dsp"

int sample_rate = 48000;
int sinebuf[48];
short buf[1024];
pthread_t tid[2];
int minibuf[1];
int is_playing;

typedef struct{
	int freq;
	int amp;
} sound_wave;

/*void *generate_tone(void *arg){
	sound_wave *wave = arg;
	int freq = wave->freq;
	int amp = wave->amp;
  static unsigned int phase = 0;	// Phase of the sine wave 
  unsigned int p;
  int i;		// 1024 samples/write is a safe choice 

  int outsz = sizeof (buf) / 2;

  /*static int sinebuf[48] = {

    0, 4276, 8480, 12539, 16383, 19947, 23169, 25995,
    28377, 30272, 31650, 32486, 32767, 32486, 31650, 30272,
    28377, 25995, 23169, 19947, 16383, 12539, 8480, 4276,
    0, -4276, -8480, -12539, -16383, -19947, -23169, -25995,
    -28377, -30272, -31650, -32486, -32767, -32486, -31650, -30272,
    -28377, -25995, -23169, -19947, -16383, -12539, -8480, -4276
  };*/
	/*int cur_amp = amp;
	for(i = 0; i < 48; i++){
		if(i%freq == 0){
			cur_amp = amp;
		}
		else if(i%freq >= freq/2){ //do the math to translate frequency
			cur_amp += amp/freq;
		}
		else{
			cur_amp -= amp/freq;
		}
		sinebuf[i] = cur_amp;
	}

  for (i = 0; i < outsz; i++)
    {

      p = (phase * sample_rate) / 48000;

      phase = (phase + 1) % 4800;
      buf[i] = sinebuf[p % 48];
    }
}*/

void *generate_tone(void *arg){
	printf("GENERATING");
	sound_wave *w = arg;
	printf("%d\n", w->freq);
}

void *write_buffer(void *arg){
	int *fd = arg;
	//trying to use is_playing as a global var, maybe should make a static var
	while(is_playing == 1){
		minibuf[0] = 16000;
		if (write (*fd, &minibuf, sizeof (minibuf)) != sizeof (buf))
			{
			perror ("Audio write");
			}
		minibuf[0] = 0;
		if (write (*fd, &minibuf, sizeof (minibuf)) != sizeof (buf))
			{
			perror ("Audio write");
			}
		minibuf[0] = -16000;
		if (write (*fd, &minibuf, sizeof (minibuf)) != sizeof (buf))
			{
			perror ("Audio write");
			}
		minibuf[0] = 0;
		if (write (*fd, &minibuf, sizeof (minibuf)) != sizeof (buf))
			{
			perror ("Audio write");
			}
	}
}

int main(void){
	int seqfd = open(MIDI_DEVICE, O_RDONLY);
	unsigned char inpacket[4];

	if(seqfd == -1){
		printf("ERROR");
	}
	else{
		printf("Connection success!\n");
	}
	int fd_out = open(SPEAKER_OUT, O_WRONLY);
	//int play_thread = pthread_create(&tid[0], NULL, write_buffer, &fd_out);
	while(1){
		read(seqfd, &inpacket, sizeof(inpacket));
		if(inpacket[0] == 144){
			printf("--------------\n");
			printf("0:%d\n", inpacket[0]);
			printf("1:%d\n", inpacket[1]);
			printf("2:%d\n", inpacket[2]);
			printf("3:%d\n", inpacket[3]);
			printf("--------------\n");
			sound_wave wave;
			wave.freq = inpacket[1]*144;
			wave.amp = 32000;
			//int tone_thread = pthread_create(&tid[1], NULL, generate_tone, &wave);
			is_playing = 1;
		}
		else if(inpacket[0] == 128){
			sound_wave wave;
			wave.freq = 1;
			wave.amp = 0;
			//int tone_thread1 = pthread_create(&tid[2], NULL, generate_tone, &wave);
			is_playing = 0;
		}
	}
}
