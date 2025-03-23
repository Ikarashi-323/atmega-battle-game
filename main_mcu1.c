#include<avr\io.h> 
#include<util/delay.h> 
#include<stdint.h> 
 
//LED関数	LEDの点灯数を制御 
void LED(int v, int w) 
{ 
int led[4]={0b000, 0b001, 0b011, 0b111};  //点灯数{0,1,2,3} 
int p_1 = led[v];  //1p点灯数 
int p_2 = led[w]*8;  //2p点灯数 
int led_bit = 0b0*64 + p_2 +p_1;  //ポートCから出力する値 
_delay_ms(1000); 
PORTC = led_bit; 
} 
 
//ボタン関数	ボタン入力されたら値を返す 
int button1(int t)  //tの値がボタン入力受付時間 
{ 
int q=0; 
int t_1=0; 
while(t_1<t)  //t秒を超えたら抜け出す 
{ 
if(bit_is_set(PINB,PB0))  //1P UP 
{ 
q=1; 
break;  //ボタン入力があったら強制的にwhileから抜ける 
} 
else if(bit_is_set(PINB,PB1))  //1P Stay 
{ 
q=2; 
break; 
} 
else if(bit_is_set(PINB,PB2))  //1P Down 
{ 
q=3; 
break; 
} 
_delay_ms(10); 
t_1 = t_1+10;  //10msずつ加算 
} 
return q;  //入力なし=０, UP=1, Stay=2, Down=3　が戻り値 
} 
 
int button2(int j)   
{ 
int s=0; 
int t_2=0; 
while(t_2<j) 
{ 
if(bit_is_set(PINB,PB3))  //2P UP 
{ 
s=1; 
break; 
} 
else if(bit_is_set(PINB,PB4))  //2P Stay 
{ 
s=2; 
break; 
} 
else if(bit_is_set(PINB,PB5))  //2P Down 
{ 
s=3; 
break; 
} 
_delay_ms(10); 
t_2 = t_2+10; 
} 
return s; 
} 
 
//メインの文 
int main(void) 
{ 
int m=3, n=3;  //HP (1P=m, 2P=n) 
int k=2;  //ターン管理値（偶数なら１P攻撃、奇数なら２P攻撃） 
DDRC=0xff; 
PORTC=0xff; 
DDRB=0x00; 
 
while(m*n>0)  //決着つくまでループ 
{     
if(k%2==0)  //1p攻撃 
{ 
int x=0, y=0;  //ボタン入力値（1p=x、２p=ｙ） 
PORTB=0xff; 
//ターン開始合図// 
LED(3 ,0); 
_delay_ms(500); 
LED(2 ,0); 
_delay_ms(500);     
LED(1 ,0); 
_delay_ms(500); 
LED(m, n); 
_delay_ms(500); 
 
x=button1(30000);  //1pボタン入力受付（30秒） 
if(x==1)  //UP 
{ 
PORTD=0b00000010;  //プロペラ時計回り 
_delay_ms(50); 
} 
else if(x==2)  //Stay 
{ 
_delay_ms(50); 
} 
else if(x==3)  //Down 
{ 
PORTD=0b00000001;  //プロペラ反時計回り 
_delay_ms(100);             
} 
_delay_ms(1000);   
 
y=button2(3000);  //2Pボタン入力受付（3秒） 
 
_delay_ms(1000);  //判定までの時間 
 
if(x==y)  //ボタン入力一致 
{ 
LED(m, n); 
} 
else if(x!=y)  //ボタン入力不一致 
{ 
n=n-1;  //2PのHPが減る。 
_delay_ms(1000); 
for(int a=0; a<4; a++) 
{     
LED(0,0); 
_delay_ms(500); 
LED(m,n); 
_delay_ms(500); 
} 
} 
 
PORTD=0b11111111;  //プロペラ回転停止 
PORTD=0b00000000; 
_delay_ms(1000); 
k=k+1;  //ターン管理値を奇数にする。 
_delay_ms(500); 
} 
 
else if(k%2==1)  //2P攻撃 
{ 
int x=0, y=0; 
PORTB=0xff; 
//ターン開始合図// 
LED(0,3); 
_delay_ms(500); 
LED(0,2); 
_delay_ms(500);     
LED(0,1); 
_delay_ms(500); 
LED(m, n); 
_delay_ms(500); 
LED(0,0); 
 
y=button2(30000);  //2Pボタン入力受付 
 
//動作安定のため、マイコン１では２ｐのプロペラは回転させない。 
 
x=button1(3000);  //1Pボタン入力受付（3秒） 
 
_delay_ms(1000);  //判定までの時間 
 
if(x==y)  //ボタン一致 
{ 
LED(m, n); 
} 
else if(x!=y)  //ボタン不一致 
{ 
m=m-1; 
_delay_ms(1000); 
for(int b=0; b<4; b++) 
{     
LED(0,0); 
_delay_ms(500); 
LED(m,n); 
_delay_ms(500); 
} 
} 
_delay_ms(1000); 
k=k+1;  //ターン管理値を偶数に 
_delay_ms(500); 
} 
} 
 
//終了合図 
for(int c=0; c<17; c++) 
{ 
int d=c%4; 
LED(d, d); 
_delay_ms(300); 
} 
return (0); 
} 
