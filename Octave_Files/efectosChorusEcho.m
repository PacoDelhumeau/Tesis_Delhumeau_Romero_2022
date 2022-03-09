clc;clear;clc;clear;
Fs = 8000; %Frecuencia de muestreo de voz
s = 1;     %Segundos de grabación
samples = Fs*s;
p = 0.01;   %tiempo de señal del pulso - 10 ms
tChorus = 0.03;  %tiempo de retardo de Chorus -30 ms - muestra num 240
tEcho = 0.1;     %tiempo de retardo de Echo - 100 ms
t = 1/Fs:1/Fs:s;
Amp = 1;

x = [Amp*ones(1,p*Fs) zeros(1,(s/2-p)*Fs) Amp*ones(1,p*Fs) zeros(1,(s/2-p)*Fs)];
xBuffer = zeros(1,s*Fs);
for i = 1:samples
    for j = samples:-1:2
       xBuffer(j) = xBuffer(j-1);
    end
    xBuffer(1) = x(i);      
    yChorus(i) = x(i)+xBuffer(Fs*tChorus)*0.5;    
    yEcho(i) = x(i)+xBuffer(Fs*tEcho)*0.5; 
end
figure;
subplot(3,1,1);
plot(t,x)
title('Señal pulso de entrada');
ylabel('Función Pulso');
subplot(3,1,2);
plot(t,yChorus)
title('Efectos de retardo en pulso');
ylabel('Señal Chorus');
subplot(3,1,3);
plot(t,yEcho)
ylabel('Señal Echo');
xlabel('Tiempo');

fo = 20; %Señal senoidal a 20 Hz
a1 = 2*cos(2*pi*fo/samples);
a2 = 1;
bo = sin(2*pi*fo/samples);
xSen(1) = bo;
xSen(2) = a1*xSen(1);
for n = 3:samples
    xSen(n)=a1*xSen(n-1)-a2*xSen(n-2);
end
xBuffer = zeros(1,s*Fs);
for i = 1:samples
    for j = samples:-1:2
       xBuffer(j) = xBuffer(j-1);
    end
    xBuffer(1) = xSen(i);      
    yChorus(i) = xSen(i)+xBuffer(Fs*tChorus)*0.5;    
    yEcho(i) = xSen(i)+xBuffer(Fs*tEcho)*0.5; 
end
figure;
subplot(3,1,1);
plot(t,xSen)
title('Señal senoidal de entrada');
ylabel('Función Senoidal');
subplot(3,1,2);
plot(t,yChorus)
title('Efectos de retardo en pulso');
ylabel('Señal Chorus');
subplot(3,1,3);
plot(t,yEcho)
ylabel('Señal Echo');
xlabel('Tiempo');

