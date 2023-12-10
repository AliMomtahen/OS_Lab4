// Console input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"




#define KEY_UP 226
#define KEY_DOWN 227
#define KEY_LEFT 228

static void consputc(int);

static int panicked = 0;

static struct {
  struct spinlock lock;
  int locking;
} cons;

static void
printint(int xx, int base, int sign)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    consputc(buf[i]);
}
//PAGEBREAK: 50

// Print to the console. only understands %d, %x, %p, %s.
void
cprintf(char *fmt, ...)
{
  int i, c, locking;
  uint *argp;
  char *s;

  locking = cons.locking;
  if(locking)
    acquire(&cons.lock);

  if (fmt == 0)
    panic("null fmt");

  argp = (uint*)(void*)(&fmt + 1);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      consputc(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if(c == 0)
      break;
    switch(c){
    case 'd':
      printint(*argp++, 10, 1);
      break;
    case 'x':
    case 'p':
      printint(*argp++, 16, 0);
      break;
    case 's':
      if((s = (char*)*argp++) == 0)
        s = "(null)";
      for(; *s; s++)
        consputc(*s);
      break;
    case '%':
      consputc('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      consputc('%');
      consputc(c);
      break;
    }
  }

  if(locking)
    release(&cons.lock);
}

void
panic(char *s)
{
  int i;
  uint pcs[10];

  cli();
  cons.locking = 0;
  // use lapiccpunum so that we can call panic from mycpu()
  cprintf("lapicid %d: panic: ", lapicid());
  cprintf(s);
  cprintf("\n");
  getcallerpcs(&s, pcs);
  for(i=0; i<10; i++)
    cprintf(" %p", pcs[i]);
  panicked = 1; // freeze other CPU
  for(;;)
    ;
}

//PAGEBREAK: 50
#define BACKSPACE 0x100
#define CRTPORT 0x3d4
static ushort *crt = (ushort*)P2V(0xb8000);  // CGA memory

static void
cgaputc(int c)
{
  int pos;

  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT+1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT+1);


  int ii=pos;

  if(c == '\n')
    pos += 80 - pos%80;
  else if(c == BACKSPACE){
    if(pos > 0){ --pos;
      while( ii < 80*25){
      crt[ii-1] = crt[ii];
      ii++;
      
    }
    }
  } else{
    //crt[pos++] = (c&0xff) | 0x0700;  // black on white
    pos++;
    unsigned int cc=(c&0xff) | 0x0700  , ccc;
    while( ii < 80*25){
      ccc = crt[ii];
      crt[ii] = cc;
      ii++;
      cc =ccc;
    }
  }
  if(pos < 0 || pos > 25*80)
    panic("pos under/overflow");

  if((pos/80) >= 24){  // Scroll up.
    memmove(crt, crt+80, sizeof(crt[0])*23*80);
    pos -= 80;
    memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
  }

  

  outb(CRTPORT, 14);
  outb(CRTPORT+1, pos>>8);
  outb(CRTPORT, 15);
  outb(CRTPORT+1, pos);
  //crt[pos] = ' ' | 0x0700;
}

void
consputc(int c)
{
  if(panicked){
    cli();
    for(;;)
      ;
  }

  if(c == BACKSPACE){
    uartputc('\b'); uartputc(' '); uartputc('\b');
  } else
    uartputc(c);
  cgaputc(c);
}

#define INPUT_BUF 128
struct {
  char buf[INPUT_BUF];
  uint r;  // Read index
  uint w;  // Write index
  uint e;  // Edit index
} input;

//int end_line =0;

#define C(x)  ((x)-'@')  // Control-x

struct node_str {
  struct node_str *next;
  char *str;
};

int check_change = 0;

struct  {
  char command_list[10][INPUT_BUF];
  int size;
  int current;
  int head;

} new_stack;


 char temppp[INPUT_BUF] = "Farbod";



void push_new_stack(char *str){
    
      new_stack.head = (new_stack.head + 1) % 10;
      
      for(int i=0 ; i< INPUT_BUF ; i++)
      new_stack.command_list[new_stack.head][i] = str[i];
    
      

      if(new_stack.size < 10)
        new_stack.size++;
    

    if(new_stack.head >= 2)
      temppp[2] = 'L';
    new_stack.current = -1;
}

char * get_befor_cmd(){
    if(new_stack.current >= new_stack.size -1)
      return (char *)0;
    
    new_stack.current++; 
    new_stack.current= new_stack.current % 10;

    char * ret = new_stack.command_list[(new_stack.head - new_stack.current + 10 ) % 10];

    if(sizeof(new_stack.command_list[0]) >= 1 )
      temppp[1]++;
    return ret;
    
}

char *get_next_cmd(){
  if(new_stack.current <= 0)
    return 0;
  else{
      new_stack.current += 9; new_stack.current %= 10;

      char * ret = new_stack.command_list[(new_stack.head- new_stack.current + 10) %10];
      return ret;
  
  }
}

void go_left(){
  int pos;

  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT+1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT+1);

  if(pos > 0)
    pos--;
  
  


  outb(CRTPORT, 14);
  outb(CRTPORT+1, pos>>8);
  outb(CRTPORT, 15);
  outb(CRTPORT+1, pos);
  
}

void go_right(){
  int pos;

  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT+1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT+1);

  if(pos < 25*80)
    pos++;
  
  


  outb(CRTPORT, 14);
  outb(CRTPORT+1, pos>>8);
  outb(CRTPORT, 15);
  outb(CRTPORT+1, pos);
  
}
uint end_l;


void clear_line(){

  while(1){
    input.e++;
    if(input.e == end_l+1){
      input.e--;
      break;}

    else
      go_right();
  }
   while(end_l != input.w &&
            input.buf[(input.e-1) % INPUT_BUF] != '\n'){
        
        end_l--;
        consputc(BACKSPACE);
      }
      input.e = end_l;
      

}


void clear_screen(){
  int pos;
  outb(CRTPORT, 14);
  pos = inb(CRTPORT+1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT+1);

  
  while((pos/80) > 0){  // 
    memmove(crt, crt+80, sizeof(crt[0])*23*80);
    pos -= 80;
    memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
  }

  outb(CRTPORT, 14);
  outb(CRTPORT+1, pos>>8);
  outb(CRTPORT, 15);
  outb(CRTPORT+1, pos);
  crt[pos] = ' ' | 0x0700;
}




void
consoleintr(int (*getc)(void))
{
  int c, doprocdump = 0;
  if(check_change == 0){
    new_stack.head = -1;
    new_stack.size = 0;
    new_stack.current = -1;
    check_change = 1;
    end_l = input.e;

  }
  char * cmd_str;

  //end_line = input.e;
  //int first_line = input.w;
  acquire(&cons.lock);
  while((c = getc()) >= 0){
    // if(c!=C('B')){
    //   for(int i=end_l+1 ; (input.e) < i ; i--){
    //     input.buf[i % INPUT_BUF] = input.buf[(i-1) % INPUT_BUF];
    //   }
    // }
    switch(c){
    case C('P'):  // Process listing.
      // procdump() locks cons.lock indirectly; invoke later
      doprocdump = 1;
      break;
    case (KEY_UP):
      cmd_str = get_befor_cmd();
      // if(new_stack.size == 1)
      //   temppp[1] = 'E';
      if(!cmd_str){
         break;
      }
      clear_line();
      for(int i=0 ; cmd_str[i] != '\0' ; i++){
        int cc = (int) cmd_str[i];
        input.buf[input.e++ % INPUT_BUF] = cc;
        end_l++;
        consputc(cc);
      }
      break;

    case C('L'):
      clear_line();
      clear_screen();
      break;
    case (KEY_DOWN):
      cmd_str = get_next_cmd();
      if(!cmd_str){
         break;
      }
      clear_line();
      for(int i=0 ; cmd_str[i] != '\0' ; i++){
        int cc = (int) cmd_str[i];
        input.buf[input.e++ % INPUT_BUF] = cc;
        end_l++;
        consputc(cc);
      }
      break;
    case C('U'):  // Kill line.
      clear_line();
      input.e = end_l;
      break;
    case C('H'): case '\x7f':  // Backspace
      if(input.e != input.w){
        input.e--;
        for(int i=input.e ; i < end_l ; i++){
          input.buf[i % INPUT_BUF] = input.buf[(i+1)%INPUT_BUF];
        }
        end_l--;
        consputc(BACKSPACE);
      }
      break;
    case C('F'):
      input.e++;
      if(input.e == end_l+1)
        input.e--;
      else
        go_right();
      break;
    case C('B'):
      
      // most have if 
          input.e--;
          if(input.e == input.w -1)
            input.e++;
          else
            go_left();


      break;
    default:
      if(c != 0 && input.e-input.r < INPUT_BUF){
        c = (c == '\r') ? '\n' : c;
        int ii = input.e;
        int cc=c;
        int ccc=c;
        end_l++;

        if(c != '\n'){
          
          while(ii != end_l){

            ccc = input.buf[ii % INPUT_BUF];
            input.buf[ii++ % INPUT_BUF] = cc;
            cc = ccc;
          }
        }
        else{
          input.buf[(end_l -1) % INPUT_BUF] = c;
        }
        input.e++;
        if(c == '\n')
          input.e = end_l;
        // if(c == '\n')
        //   temppp[2] = 'G';
        consputc(c);
        
        if(c == '\n' || c == C('D') || end_l == input.r+INPUT_BUF){
          
          ////
          char str[INPUT_BUF] = "";
          // int ff = input.e;
          if(c == '\n'){
          // while(ff != input.w &&
          //   input.buf[(ff-1) % INPUT_BUF] != '\n'){
          //     ff--;
          //   }
              int ind=0;
              int i = input.w;
              // while(i < end_l){
              //     str[ind] = input.buf[i%INPUT_BUF];  
              //     ind++;
              //     i++;
              // }
              // i++;
              for(int j=i ; j<end_l-1;j++){
                str[ind] = input.buf[j];  
                ind++;
              }
              //      123a56
              str[ind] = '\0';

              push_new_stack(str);
              temppp[3] = 'B';
          }
              
              input.w = end_l;
              wakeup(&input.r);
        }
      }
      break;
    }
  }
  release(&cons.lock);
  if(doprocdump) {
    procdump();  // now call procdump() wo. cons.lock held
  }
}

int
consoleread(struct inode *ip, char *dst, int n)
{
  uint target;
  int c;

  iunlock(ip);
  target = n;
  acquire(&cons.lock);
  while(n > 0){
    while(input.r == input.w){
      if(myproc()->killed){
        release(&cons.lock);
        ilock(ip);
        return -1;
      }
      sleep(&input.r, &cons.lock);
    }
    c = input.buf[input.r++ % INPUT_BUF];
    if(c == C('D')){  // EOF
      if(n < target){
        // Save ^D for next time, to make sure
        // caller gets a 0-byte result.
        input.r--;
      }
      break;
    }
    *dst++ = c;
    --n;
    if(c == '\n')
      break;
  }
  release(&cons.lock);
  ilock(ip);

  return target - n;
}

int
consolewrite(struct inode *ip, char *buf, int n)
{
  int i;

  iunlock(ip);
  acquire(&cons.lock);
  for(i = 0; i < n; i++)
    consputc(buf[i] & 0xff);
  release(&cons.lock);
  ilock(ip);

  return n;
}

void
consoleinit(void)
{
  initlock(&cons.lock, "console");

  devsw[CONSOLE].write = consolewrite;
  devsw[CONSOLE].read = consoleread;
  cons.locking = 1;

  ioapicenable(IRQ_KBD, 0);
}

