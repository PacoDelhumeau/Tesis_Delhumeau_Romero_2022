clc;clear;
Fs = 8000; %Frecuencia de muestreo de voz
s = 2;     %Segundos de grabación
samples= Fs*s;
p = 0.01;   %tiempo de señal del pulso - 10 ms
tReverb = 0.1;     %tiempo de retardo de Echo - 100 ms
t = 1/Fs:1/Fs:s;
%Señal de entrada pulso
Amp = 1;

%x = [Amp*ones(1,p*Fs) zeros(1,(s/2-p)*Fs) Amp*ones(1,p*Fs) zeros(1,(s/2-p)*Fs)];
x = [Amp*ones(1,p*Fs) zeros(1,(s-p)*Fs)];
%Señal exponencial
exp = 1:1:Fs;
revNoiseFloor = 1/1000;
revTime = 1;             %Tiempo de reverberación 
a = (revNoiseFloor)^(1/(Fs * revTime));
fexp1 = a.^exp;
exp = 1:1:Fs*s;
revTime = 2;             %Tiempo de reverberación 
a = (revNoiseFloor)^(1/(Fs * revTime));
fexp2 = a.^exp;

% figure;
% plot(t,fexp)
% title('Función exponencial');
% ylabel('Señal');
% xlabel('Tiempo');

xBuffer = zeros(1,s*Fs);
for i = 1:samples
    for j = samples:-1:2
       xBuffer(j) = xBuffer(j-1);
    end
    xBuffer(1) = x(i);  
    yReverb1(i) = x(i);
    if (i > Fs*tReverb)
        for k = Fs/10:Fs/10:Fs
            yReverb1(i) = yReverb1(i) + xBuffer(k)*fexp1(k);
        end
    end
    yReverb2(i) = x(i);
    if (i > Fs*tReverb)
        for k = Fs/10:Fs/10:Fs*s
            yReverb2(i) = yReverb2(i) + xBuffer(k)*fexp2(k);
        end
    end
end
figure;
subplot(3,1,1);
plot(t,x)
title('Señal pulso de entrada');
ylabel('Función Pulso');
subplot(3,1,2);
plot(t,yReverb1)
title('Efectos de retardo en pulso');
ylabel('Señal Reverb 1s');
subplot(3,1,3);
plot(t,yReverb2)
ylabel('Señal Reverb 2s');
xlabel('Tiempo');

fo = 20; %Señal senoidal a 20 Hz
a1 = 2*cos(2*pi*fo/samples);
a2 = 1;
bo = sin(2*pi*fo/samples);
xSen(1) = bo;
xSen(2) = a1*xSen(1);
for n = 3:Fs
    xSen(n)=a1*xSen(n-1)-a2*xSen(n-2);
end
xSen = [xSen, zeros(1,Fs)];
xBuffer = zeros(1,s*Fs);
for i = 1:samples
    for j = samples:-1:2
       xBuffer(j) = xBuffer(j-1);
    end
    xBuffer(1) = xSen(i);  
    yReverb1(i) = xSen(i);
    if (i > Fs*tReverb)
        for k = Fs/10:Fs/10:Fs
            yReverb1(i) = yReverb1(i) + xBuffer(k)*fexp1(k);
        end
    end
    yReverb2(i) = xSen(i);
    if (i > Fs*tReverb)
        for k = Fs/10:Fs/10:Fs*s
            yReverb2(i) = yReverb2(i) + xBuffer(k)*fexp2(k);
        end
    end
end
figure;
subplot(3,1,1);
plot(t,xSen)
title('Señal senoidal de entrada');
ylabel('Función Senoidal');
subplot(3,1,2);
plot(t,yReverb1)
title('Efectos de Reverb en señal senoidal');
ylabel('Reverb 1s');
subplot(3,1,3);
plot(t,yReverb2)
ylabel('Reverb 2');
xlabel('Tiempo');

Q = 13;
for k = Fs/10:Fs/10:Fs
    vectExp1(k/800)=int16((fexp1(k)*2^Q-1));
end
for k = Fs/10:Fs/10:Fs*2
    vectExp2(k/800)=int16((fexp2(k)*2^Q-1));
end
txt = fopen('exponencialesReverb.h','wt');
fprintf(txt,'//Header con los coeficientes de los exponenciales de efecto Reverb en Q%i\n',Q);
fprintf(txt,'#pragma DATA_SECTION(expRev1,"coefFIR");\n');
fprintf(txt,'#pragma DATA_SECTION(expRev2,"coefFIR");\n');
fprintf(txt,'//Exponencial Reverb 1s\n');
fprintf(txt,'int vectExp1[]={%i',vectExp1(1));
fprintf(txt,',\n\t%i',vectExp1(2:length(vectExp1)));
fprintf(txt,'};');
fprintf(txt,'//Exponencial Reverb 2s\n');
fprintf(txt,'int vectExp2[]={%i',vectExp2(1));
fprintf(txt,',\n\t%i',vectExp2(2:length(vectExp2)));
fprintf(txt,'};');
fclose(txt);