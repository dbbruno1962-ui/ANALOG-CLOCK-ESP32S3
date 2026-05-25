# ANALOG_CLOCK_normale

Progetto di orologio basato su ESP32S3 dev module, display tft sensore dht11 cam ov.....
l'hardware è composto da MCU Goouuu con zif cam integrata e brekboard Goouuu, display ILI9488_18bit
MA IL CONFIG.H è COMPLETAMENTE PERSONALIZZABILE A SECONDA DELL'HARDWARE CHE VOLETE UTILIZZARE
non è sicuramente adattabile ad una normale Esp32 non S3, a meno di modificare il processo utilizzandone
più di 2 e ripartendo i processi, ma l'attuale progetto prevede l'utilizzo di un cloud simulato su hostweb linux
quindi anche adattabile a più periferiche, in seguito aggiungerò anche i php necessari
Hardware utilizzato:
https://www.temu.com/goods.html?_bg_fs=1&goods_id=601103835226651&parent_order_sn=PO-098-16918161423993974&_oak_order_sn=098-16918255795833974&_oak_goods_num=1&sku_id=17611805820709&_x_vst_scene=adg&_x_ads_csite=pc_bottom&_x_ads_channel=kol_affiliate&_x_cid=2008346202kol_affiliate&_x_campaign=affiliate&_x_sessn_id=ltu1pw7qm1&refer_page_name=bgt_order_detail&refer_page_id=10045_1779733705293_eodsno04r8&refer_page_sn=10045
io ho montato un 3.5 pollici, ma la resa effettivamente migliore è con un 
4 pollici https://it.aliexpress.com/item/1005008871784622.html?spm=a2g0o.order_list.order_list_main.68.5f903696zqTUv6&gatewayAdapt=glo2ita

## UI

La UI usa LVGL 8.3.0 con quadrante analogico personalizzabile tramite asset grafici.
Sono previsti tema giorno e tema notte.

- quadrante giorno: fondo chiaro e numeri neri
- quadrante notte: fondo scuro e numeri chiari
- lancette personalizzabili modificando gli asset grafici
- colonna laterale con secondi, stato WiFi, ora digitale, temperatura, umidità e data

### Librerie e core usati

Questo progetto è stato sviluppato e testato con versioni specifiche delle librerie.
LVGL è fissata alla versione 8.3.0: versioni più recenti possono richiedere modifiche importanti a macro, driver display, flush callback e API grafiche.

- LVGL: 8.3.0
- ESP32 by Espressif Systems: 3.3.5

#### Configurazione board

Board: ESP32S3 Dev Module  
Porta: COM9, variabile in base al PC

- USB CDC On Boot: Disabled
- CPU Frequency: 240MHz (WiFi)
- Core Debug Level: None
- USB DFU On Boot: Disabled
- Erase All Flash Before Sketch Upload: Disabled (*)
- Events Run On: Core 1
- Flash Mode: QIO 80MHz
- Flash Size: 16MB (128Mb)
- JTAG Adapter: Integrated USB JTAG
- Arduino Runs On: Core 1
- USB Firmware MSC On Boot: Disabled
- Partition Scheme: 16M Flash (3MB APP/9.9MB FATFS)
- PSRAM: OPI PSRAM
- Upload Mode: UART0 / Hardware CDC
- Upload Speed: 921600
- USB Mode: Hardware CDC and JTAG
- Zigbee Mode: Disabled

(*) Per la prima installazione o dopo modifiche importanti a NVS, partizioni o profili WiFi,
impostare temporaneamente:

- Erase All Flash Before Sketch Upload: Enabled

Dopo la prima flash riuscita, può essere riportato su Disabled.

Bruno
