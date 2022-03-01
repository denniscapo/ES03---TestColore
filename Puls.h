/** ****************************************************************************************
* @file Puls.h
* @brief Libreria contenente tutte le funzoni utili per utilizzare e gestire i pulsanti
* 
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22 inserite funzioni per gestire i pulsanti
*/


 /** ****************************************************************************************
* @brief Inizializzazione dei 5 pulsanti
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwInit(void);

 /** ****************************************************************************************
* @brief Inizializzazione pulsante centrale
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwCenterInit(void);

 /** ****************************************************************************************
* @brief Inizializzazione pulsante sinistro
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwLeftInit(void);

 /** ****************************************************************************************
* @brief Inizializzazione pulsante destro
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwRightInit(void);

 /** ****************************************************************************************
* @brief Inizializzazione pulsante superiore
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwUpInit(void);

 /** ****************************************************************************************
* @brief Inizializzazione pulsante inferiore
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwDownInit(void);

 /** ****************************************************************************************
* @brief Verifica pressione pulsante centrale
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwCenterPress(void);

 /** ****************************************************************************************
* @brief Verifica pressione pulsante sinistro
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwLeftPress(void);

 /** ****************************************************************************************
* @brief Verifica pressione pulsante destro
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwRightPress(void);

 /** ****************************************************************************************
* @brief Verifica pressione pulsante superiore
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwUpPress(void);

 /** ****************************************************************************************
* @brief Verifica pressione pulsante inferiore
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwDownPress(void);

 /** ****************************************************************************************
* @brief Test dei 5 pulsanti
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwTest(void);

 /** ****************************************************************************************
* @brief *VECCHIA VERSIONE* Verifico click pulsante sinistro (click=pressione+rilascio)
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwLeftClickOld(void);

 /** ****************************************************************************************
* @brief *VECCHIA VERSIONE* Verifico click pulsante destro (click=pressione+rilascio)
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwRightClickOld(void);		   //click = pressione e rilascio

 /** ****************************************************************************************
* @brief Verifico click pulsante destro (click=pressione+rilascio)
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwRightClick(void);

 /** ****************************************************************************************
* @brief Verifico click pulsante sinistro (click=pressione+rilascio)
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwLeftClick(void);

 /** ****************************************************************************************
* @brief Verifico click pulsante sinistro (click=pressione+rilascio) con variabili globali
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void SwLeftClickTask(void);

 /** ****************************************************************************************
* @brief Verifico doppio click pulsante destro (click=pressione+rilascio)
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwRight2Click(void);		 //doppio click

 /** ****************************************************************************************
* @brief *VECCHIA VERSIONE* Verifico doppio click pulsante destro (click=pressione+rilascio)
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwRight2ClickOld(void);		 //doppio click

 /** ****************************************************************************************
* @brief Verifico pressione lunga pulsante inferiore (pressione > 3s)
* @retval return=1 pulsante premuto, return=0 pulsante non premuto
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
int SwDownLPress(void);     //pressione lunga > 3 sec

 /** ****************************************************************************************
* @brief Lampeggio del led con tempo base 2000ms, ad ogni click velocizzo di 100ms
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void LedVerdeSwTask(void);

 /** ****************************************************************************************
* @brief Testo le diverse pressioni dei pulsanti
* @author Dennis Capozziello
* @date 28/01/22
* @version 1.0 28/01/22
*/
void TestPressSw(void);
