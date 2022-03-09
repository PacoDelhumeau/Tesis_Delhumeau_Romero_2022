clc;clear;clc;clear;
Fs = 8000; %Frecuencia de muestreo de voz
s = 1;     %Segundos de grabación
samples = Fs*s;
t = 1/Fs:1/Fs:s;

fo = 20; %Señal senoidal a 20 Hz
a1 = 2*cos(2*pi*fo/samples);
a2 = 1;
bo = sin(2*pi*fo/samples);
xSen(1) = bo;
xSen(2) = a1*xSen(1);
for n = 3:samples
    xSen(n)=a1*xSen(n-1)-a2*xSen(n-2);
end

for i = 1:samples
    if ( abs(xSen(i))<(1/3) )
        xSoftOverdrive(i) = 2*xSen(i);
    elseif ( abs(xSen(i))<(2/3) )
        if (xSen(i)>0)
            xSoftOverdrive(i) = 1-(((2-3*xSen(i))^2)/3);
        else
            xSoftOverdrive(i) = -(1-(((2-3*abs(xSen(i)))^2)/3));
        end
    else 
        if (xSen(i)>0)
            xSoftOverdrive(i) = 1;
        else 
            xSoftOverdrive(i) = -1;
        end
    end
end
gain = 5;
mix = 1;
for i = 1:samples
    q=xSen(i)*gain;
    z=sign(q).*(1-exp(-abs(q)));
    xHardOverdrive(i)=mix*z+(1-mix)*xSen(i);
end
xfft = fft(xSen);
xSCfft = fft(xSoftOverdrive);
xHCfft = fft(xHardOverdrive);
figure;
subplot(3,1,1);
plot(t,xSen)
title('Señal senoidal de entrada');
ylabel('Función Senoidal');
subplot(3,1,2);
plot(t,xSoftOverdrive)
title('Efectos de Overdrive en señal senoidal');
ylabel('Soft Clipping');
subplot(3,1,3);
plot(t,xHardOverdrive)
ylabel('Hard Clipping');
xlabel('Tiempo');
figure;
subplot(3,1,1);
stem(0:4000,abs(xfft(1:4001)));
title('Espectro senoidal de entrada');
ylabel('Espectro Senoidal');
subplot(3,1,2);
stem(0:4000,abs(xSCfft(1:4001)));
title('Efectos de Overdrive en señal senoidal');
ylabel('Soft Clipping');
subplot(3,1,3);
stem(0:4000,abs(xHCfft(1:4001)));
ylabel('Hard Clipping');
xlabel('Frecuencia');