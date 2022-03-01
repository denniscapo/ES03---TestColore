# es01-rtos 
## PROGETTO NASTRO TRASPORTATORE-Sensore ad ultrasuoni
![Schemi per il collegamento dell’hardware](https://github.com/denniscapo/ES03_TestColore/blob/main/Catturak.PNG)
![Immagine Sensore ad infrarossi](https://i1.wp.com/www.arduiner.com/wp-content/uploads/2013/01/14418-Infrarossi-reflective-Fotoelettrico-Pulsante-IR-Barrier-Line-Tracking-Sensore-TCRT5000L-Arduino.jpg?fit=500%2C300&ssl=1) 

**SPECIFICHE TCRT5000-IR-Sensor-Module**
- tensione operativa 		5 V
- corrente operativa 		4 mA
- frequenza 			40 Khz
- portata 			1 cm -- 4 m
- dimensioni 			43 x 20 x 17(h) mm 

Sulla scheda sensore sono presenti otto coppie di elementi che emettono luce infrarossa sul percorso e rilevano la luce infrarossa riflessa dal percorso. ***Il bianco riflette la luce e il nero assorbe la luce***. La luce a infrarossi viene irradiata sul percorso utilizzando l'elemento emettitore di luce a infrarossi. Se l'elemento di rilevamento della luce infrarossa rileva la luce infrarossa, il colore viene determinato come "bianco" e, in caso contrario, viene determinato come "nero".
La quantità di luce infrarossa emessa può essere regolata utilizzando un resistore di regolazione.

![Immagine Sensore ad infrarossi](https://erltechonline.com/img/5x-TCRT5000-Infrared-Reflectance-Sensor-Obstacle-Avoidance-Track-Module-IR-TE364-3.jpg)  

**Schemi per il collegamento dell’hardware**



![Schemi per il collegamento dell’hardware](https://github.com/denniscapo/ES03_TestColore/blob/main/Catturaf.PNG)

Il ***sensore ad infrarossi*** è necessario per il controllo del colore, tramite il quale viene verificato il colore dell’oggetto posizionato sul rullo. Il sensore è collegato tramite un cavo alla scheda ***STM32.*** Nello specifico collegato al pin ***PE10,*** il quale è settato come ***input.*** Questa task deve essere eseguita quando l’ultrasuoni rileva un oggetto. Questo meccanico di comunicazioni tra task lo gestiamo con gli eventgroup. Quando il flag dell’ ultrasuoni è attivo la task dell’ infrarossi viene eseguita e quando viene rilevato un oggetto attiva un flag che serve per l’attivazione e esecuzione delle task successive (Task smista oggetto e LCD). 

**Specifiche di Collaudo**
Quando il sensore rileva un oggetto di colore bianco che riflette il segnale al sensore ad infrarossi fa accendere un led verde, nel caso del colore nero, quindi non rilevato, il led rosso. Dopo che il sensore ad infrarossi rileva un oggetto ce un piccolo timer di mezzo secondo (500ms) in cui loggetto arriva al componente successivo. 
