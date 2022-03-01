/** ****************************************************************************************
* @file Led.h
* @brief Libreria contenente tutte le funzoni utili per utilizzare e gestire i led
* 
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22 inserite funzioni per gestire i led
*/

 /** ****************************************************************************************
* @brief Inizializzazione dei 2 led
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedInit(void);

 /** ****************************************************************************************
* @brief Inizializzazione del led rosso
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedRossoInit(void);

 /** ****************************************************************************************
* @brief Inizializzazione del led verde
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeInit(void);

 /** ****************************************************************************************
* @brief Accensione led rosso
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedRossoOn(void);

 /** ****************************************************************************************
* @brief Spegnimento led rosso
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedRossoOff(void);

 /** ****************************************************************************************
* @brief Inversione stato led rosso
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedRossoToggle(void);

 /** ****************************************************************************************
* @brief Accensione led verde
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeOn(void);

 /** ****************************************************************************************
* @brief Spegnimento led verde
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeOff(void);

 /** ****************************************************************************************
* @brief Inversione stato led verde
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeToggle(void);

 /** ****************************************************************************************
* @brief Lampeggio led rosso con parametri
* @param  ritardo=distanza tra lampeggi(in ms), ncicli=num lampeggi
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedRossoBlink(int ritardo,int ncicli);

 /** ****************************************************************************************
* @brief Lampeggio led verde con parametri
* @param  ritardo=distanza tra lampeggi(in ms), ncicli=num lampeggi
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeBlink(int ritardo,int ncicli);

 /** ****************************************************************************************
* @brief Testo i due led (10 lampeggi ogni 100ms)
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedTest(void);

 /** ****************************************************************************************
* @brief Lampeggio led verde con parametri
* @param  Ton=tempo led accesso(ms), T=tempo totale(acceso+spento)
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedRossoTask(int Ton,int T);

 /** ****************************************************************************************
* @brief Lampeggio led verde con parametri
* @param  Ton=tempo led accesso(ms), T=tempo totale(acceso+spento)
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeTask(int Ton,int T);

 /** ****************************************************************************************
* @brief Lampeggio led verde con parametro
* @param  startTime=delay per il lampeggio del led verde
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeBlinkTask(int startTime);

 /** ****************************************************************************************
* @brief Lampeggio led verde con parametro
* @param  startTime=delay per il lampeggio del led rosso
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedRossoBlinkTask(int startTime);
