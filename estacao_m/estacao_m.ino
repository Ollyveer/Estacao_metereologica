#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
//  Configuração do DHT11
#define DHTPIN 10          
#define DHTTYPE DHT11

//  Configuração do LDR
#define LDRPIN 0          
// Cria o objeto do sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Dados da rede Wi-Fi
const char* ssid = "Alfred";
const char* password = "12345678";

//  Dados do ThingSpeak 
unsigned long channelID = 3433718;
const char* writeAPIKey = "6XRGP8BZ1NVCIVKV";


WiFiClient client;

 void setup()
{
    // Inicia a comunicação com o computador
    Serial.begin(19200);

    // Inicia o sensor DHT11
    dht.begin();

    // Inicia a conexão com a rede Wi-Fi
    WiFi.begin(ssid, password);

    Serial.println("Conectando ao Wi-Fi...");

    // Espera o ESP32 conectar
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi conectado!");

    // Inicia a comunicação com o ThingSpeak
    ThingSpeak.begin(client);
}
 void loop()
{
    // Lê a temperatura em °C
    float temperatura = dht.readTemperature();

    // Lê a umidade em %
    float umidade = dht.readHumidity();

    // Lê o valor analógico do LDR
    int luminosidade = analogRead(LDRPIN);

    // Verifica se houve erro na leitura do DHT11
    if (isnan(temperatura) || isnan(umidade))
    {
        Serial.println("Erro ao ler o DHT11");
    }
    else
    {
        // Mostra os dados no Monitor Serial
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" °C");

        Serial.print("Umidade: ");
        Serial.print(umidade);
        Serial.println(" %");

        Serial.print("Luminosidade: ");
        Serial.println(luminosidade);

        // Envia os valores para os campos do ThingSpeak
        ThingSpeak.setField(1, temperatura);
        ThingSpeak.setField(2, umidade);
        ThingSpeak.setField(3, luminosidade);

        // Envia os dados
        int resposta = ThingSpeak.writeFields(channelID, writeAPIKey);

        if (resposta == 200)
        {
            Serial.println("Dados enviados");
        }
        else
        {
            Serial.print("Erro ao enviar ");
            Serial.println(resposta);
        }
    }

    // Aguarda 15 segundos antes da próxima leitura
    delay(15000);
}
