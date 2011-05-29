/* NOTE: this is valid input for Project 6, but it is NOT guaranteed
   to be valid output from Project 5 */

float data_real[1024UL];
float data_imag[1024UL];
float coef_real[1024UL];
float coef_imag[1024UL];

int main()
{
  int __temp1;
  int __temp2;
  int __temp3;
  int __temp4;
  int __temp5;
  int __temp6;
  double __temp7;
  int __temp8;
  int __temp9;
  int __temp10;
  int __temp11;
  int __temp12;
  int __temp13;
  int __temp14;
  int __temp15;
  int __temp16;
  int __temp17;
  int __temp18;
  int __temp19;
  float __temp20;
  int __temp21;
  int __temp22;
  int __temp23;
  int __temp24;
  int __temp25;
  float __temp26;
  float __temp27;
  int __temp28;
  int __temp29;
  int __temp30;
  int __temp31;
  int __temp32;
  float __temp33;
  int __temp34;
  int __temp35;
  int __temp36;
  int __temp37;
  int __temp38;
  float __temp39;
  float __temp40;
  int __temp41;
  int __temp42;
  int __temp43;
  int __temp44;
  int __temp45;
  int __temp46;
  int __temp47;
  int __temp48;
  float __temp49;
  float __temp50;
  float __temp51;
  int __temp52;
  int __temp53;
  int __temp54;
  int __temp55;
  int __temp56;
  int __temp57;
  int __temp58;
  int __temp59;
  float __temp60;
  float __temp61;
  float __temp62;
  float __temp63;
  float __temp64;
  int __temp65;
  int __temp66;
  int __temp67;
  int __temp68;
  int __temp69;
  int __temp70;
  int __temp71;
  int __temp72;
  float __temp73;
  float __temp74;
  float __temp75;
  int __temp76;
  int __temp77;
  int __temp78;
  int __temp79;
  int __temp80;
  int __temp81;
  int __temp82;
  int __temp83;
  float __temp84;
  float __temp85;
  float __temp86;
  int __temp87;
  int __temp88;
  int __temp89;
  int __temp90;
  int __temp91;
  int __temp92;
  int __temp93;
  int __temp94;
  int __temp95;
  int __temp96;
  int __temp97;
  int __temp98;
  int __temp99;
  int __temp100;
  float __temp101;
  float __temp102;
  float __temp103;
  int __temp104;
  int __temp105;
  int __temp106;
  int __temp107;
  int __temp108;
  int __temp109;
  float __temp110;
  float __temp111;
  int __temp112;
  int __temp113;
  int __temp114;
  int __temp115;
  int __temp116;
  int __temp117;
  int __temp118;
  int __temp119;
  int __temp120;
  int __temp121;
  int __temp122;
  int __temp123;
  int __temp124;
  int __temp125;
  float __temp126;
  float __temp127;
  float __temp128;
  int __temp129;
  int __temp130;
  int __temp131;
  int __temp132;
  int __temp133;
  int __temp134;
  float __temp135;
  float __temp136;
  int __temp137;
  int __temp138;
  int __temp139;
  int __temp140;
  int __temp141;
  int __temp142;
  int __temp143;
  int __temp144;
  int __temp145;
  int __temp146;
  float __temp147;
  int __temp148;
  float __temp149;
  double __temp150;
  float __temp151;
  float __temp152;
  int i;
  int j;
  int k;
  float temp_real;
  float temp_imag;
  float Wr;
  float Wi;
  int groupsPerStage = 1;
  int buttersPerGroup = (1024 / 2);
  float sum = (0.0);
  L2:
  L3:
  L4:
  L5:
  L6:
  L7:
  L8:
  i = 0;
  L10:
  __temp2 = i;
  if (__temp2 < 1024) 
    goto L11;
  goto L9;
  L11:
  __temp5 = i;
  __temp6 = i;
  __temp7 = (0.33 * (__temp6));
  data_real[__temp5] = (__temp7);
  L12:
  __temp8 = i;
  data_imag[__temp8] = (1);
  L13:
  __temp9 = i;
  coef_real[__temp9] = (1);
  L14:
  __temp10 = i;
  coef_imag[__temp10] = (1);
  L15:
  __temp4 = i;
  i = (i + 1);
  goto L10;
  L9:
  L16:
  L17:
  i = 0;
  L19:
  __temp12 = i;
  if (__temp12 < 10) 
    goto L20;
  goto L18;
  L20:
  j = 0;
  L22:
  __temp16 = j;
  __temp17 = groupsPerStage;
  if (__temp16 < __temp17) 
    goto L23;
  goto L21;
  L23:
  __temp21 = i;
  __temp22 = (1 << __temp21);
  __temp23 = (__temp22 - 1);
  __temp24 = j;
  __temp25 = (__temp23 + __temp24);
  __temp26 = (coef_real[__temp25]);
  Wr = __temp26;
  L24:
  __temp28 = i;
  __temp29 = (1 << __temp28);
  __temp30 = (__temp29 - 1);
  __temp31 = j;
  __temp32 = (__temp30 + __temp31);
  __temp33 = (coef_imag[__temp32]);
  Wi = __temp33;
  L25:
  k = 0;
  L27:
  __temp35 = k;
  __temp36 = buttersPerGroup;
  if (__temp35 < __temp36) 
    goto L28;
  goto L26;
  L28:
  __temp40 = Wr;
  __temp41 = j;
  __temp42 = (2 * __temp41);
  __temp43 = buttersPerGroup;
  __temp44 = (__temp42 * __temp43);
  __temp45 = buttersPerGroup;
  __temp46 = (__temp44 + __temp45);
  __temp47 = k;
  __temp48 = (__temp46 + __temp47);
  __temp49 = (data_real[__temp48]);
  __temp50 = (__temp40 * __temp49);
  __temp51 = Wi;
  __temp52 = j;
  __temp53 = (2 * __temp52);
  __temp54 = buttersPerGroup;
  __temp55 = (__temp53 * __temp54);
  __temp56 = buttersPerGroup;
  __temp57 = (__temp55 + __temp56);
  __temp58 = k;
  __temp59 = (__temp57 + __temp58);
  __temp60 = (data_imag[__temp59]);
  __temp61 = (__temp51 * __temp60);
  __temp62 = (__temp50 - __temp61);
  temp_real = __temp62;
  L29:
  __temp64 = Wi;
  __temp65 = j;
  __temp66 = (2 * __temp65);
  __temp67 = buttersPerGroup;
  __temp68 = (__temp66 * __temp67);
  __temp69 = buttersPerGroup;
  __temp70 = (__temp68 + __temp69);
  __temp71 = k;
  __temp72 = (__temp70 + __temp71);
  __temp73 = (data_real[__temp72]);
  __temp74 = (__temp64 * __temp73);
  __temp75 = Wr;
  __temp76 = j;
  __temp77 = (2 * __temp76);
  __temp78 = buttersPerGroup;
  __temp79 = (__temp77 * __temp78);
  __temp80 = buttersPerGroup;
  __temp81 = (__temp79 + __temp80);
  __temp82 = k;
  __temp83 = (__temp81 + __temp82);
  __temp84 = (data_imag[__temp83]);
  __temp85 = (__temp75 * __temp84);
  __temp86 = (__temp74 + __temp85);
  temp_imag = __temp86;
  L30:
  __temp87 = j;
  __temp88 = (2 * __temp87);
  __temp89 = buttersPerGroup;
  __temp90 = (__temp88 * __temp89);
  __temp91 = buttersPerGroup;
  __temp92 = (__temp90 + __temp91);
  __temp93 = k;
  __temp94 = (__temp92 + __temp93);
  __temp95 = j;
  __temp96 = (2 * __temp95);
  __temp97 = buttersPerGroup;
  __temp98 = (__temp96 * __temp97);
  __temp99 = k;
  __temp100 = (__temp98 + __temp99);
  __temp101 = (data_real[__temp100]);
  __temp102 = temp_real;
  __temp103 = (__temp101 - __temp102);
  data_real[__temp94] = __temp103;
  L31:
  __temp104 = j;
  __temp105 = (2 * __temp104);
  __temp106 = buttersPerGroup;
  __temp107 = (__temp105 * __temp106);
  __temp108 = k;
  __temp109 = (__temp107 + __temp108);
  __temp110 = temp_real;
  __temp111 = (data_real[__temp109]);
  __temp111 = (__temp111 + __temp110);
  data_real[__temp109] = __temp111;
  L32:
  __temp112 = j;
  __temp113 = (2 * __temp112);
  __temp114 = buttersPerGroup;
  __temp115 = (__temp113 * __temp114);
  __temp116 = buttersPerGroup;
  __temp117 = (__temp115 + __temp116);
  __temp118 = k;
  __temp119 = (__temp117 + __temp118);
  __temp120 = j;
  __temp121 = (2 * __temp120);
  __temp122 = buttersPerGroup;
  __temp123 = (__temp121 * __temp122);
  __temp124 = k;
  __temp125 = (__temp123 + __temp124);
  __temp126 = (data_imag[__temp125]);
  __temp127 = temp_imag;
  __temp128 = (__temp126 - __temp127);
  data_imag[__temp119] = __temp128;
  L33:
  __temp129 = j;
  __temp130 = (2 * __temp129);
  __temp131 = buttersPerGroup;
  __temp132 = (__temp130 * __temp131);
  __temp133 = k;
  __temp134 = (__temp132 + __temp133);
  __temp135 = temp_imag;
  __temp136 = (data_imag[__temp134]);
  __temp136 = (__temp136 + __temp135);
  data_imag[__temp134] = __temp136;
  L34:
  __temp38 = (k + 1);
  k = __temp38;
  goto L27;
  L26:
  __temp19 = (j + 1);
  j = __temp19;
  goto L22;
  L21:
  __temp139 = groupsPerStage;
  __temp138 = (__temp139 << 1);
  groupsPerStage = __temp138;
  L35:
  __temp142 = buttersPerGroup;
  __temp141 = (__temp142 >> 1);
  buttersPerGroup = __temp141;
  L36:
  __temp14 = (i + 1);
  i = __temp14;
  goto L19;
  L18:
  L37:
  i = 0;
  L39:
  __temp144 = i;
  if (__temp144 < 1024) 
    goto L40;
  goto L38;
  L40:
  __temp152 = sum;
  __temp148 = i;
  __temp149 = (data_real[__temp148]);
  __temp150 = (111.1 * (__temp149));
  __temp151 = (((__temp152) + __temp150));
  sum = __temp151;
  __temp146 = i;
  i = (i + 1);
  goto L39;
  L38:
  L1:
  L41:
  return sum;
}
