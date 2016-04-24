#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../pico/lib/picoapi.h"]

#define PICO_MEM_SIZE       2500000
const char * picoSupportedLangIso3[] = { "eng", "eng", "deu", "spa", "fra", "ita" };
const char * picoSupportedCountryIso3[] = { "USA", "GBR", "DEU", "ESP", "FRA", "ITA" };
const char * picoSupportedLang[] = { "en-US", "en-GB", "de-DE", "es-ES", "fr-FR", "it-IT" };
const char * picoInternalLang[] = { "en-US", "en-GB", "de-DE", "es-ES", "fr-FR", "it-IT" };
const char * picoInternalTaLingware[] = { "en-US_ta.bin", "en-GB_ta.bin", "de-DE_ta.bin", "es-ES_ta.bin", "fr-FR_ta.bin", "it-IT_ta.bin" };
const char * picoInternalSgLingware[] = { "en-US_lh0_sg.bin", "en-GB_kh0_sg.bin", "de-DE_gl0_sg.bin", "es-ES_zl0_sg.bin", "fr-FR_nk0_sg.bin", "it-IT_cm0_sg.bin" };
const char * picoInternalUtppLingware[] = { "en-US_utpp.bin", "en-GB_utpp.bin", "de-DE_utpp.bin", "es-ES_utpp.bin", "fr-FR_utpp.bin", "it-IT_utpp.bin" };
const char * PICO_VOICE_NAME = "PicoVoice";


void *          picoMemArea = NULL;
pico_System     picoSystem = NULL;
pico_Resource   picoTaResource = NULL;
pico_Resource   picoSgResource = NULL;
pico_Resource   picoUtppResource = NULL;
pico_Engine     picoEngine = NULL;
pico_Char *     picoTaFileName = NULL;
pico_Char *     picoSgFileName = NULL;
pico_Char *     picoUtppFileName = NULL;
pico_Char *     picoTaResourceName = NULL;
pico_Char *     picoSgResourceName = NULL;
pico_Char *     picoUtppResourceName = NULL;

static void cleanResources(void)
{
    if (picoEngine) {
        pico_disposeEngine(picoSystem, &picoEngine);
        pico_releaseVoiceDefinition(picoSystem, (pico_Char *)PICO_VOICE_NAME);
        picoEngine = NULL;
    }
    if (picoUtppResource) {
        pico_unloadResource(picoSystem, &picoUtppResource);
        picoUtppResource = NULL;
    }
    if (picoTaResource) {
        pico_unloadResource(picoSystem, &picoTaResource);
        picoTaResource = NULL;
    }
    if (picoSgResource) {
        pico_unloadResource(picoSystem, &picoSgResource);
        picoSgResource = NULL;
    }

    if (picoSystem) {
        pico_terminate(&picoSystem);
        picoSystem = NULL;
    }

}


int main(int argc, char *argv[])
{

    int ret;



    picoMemArea = malloc(PICO_MEM_SIZE);
    ret = pico_initialize(picoMemArea, PICO_MEM_SIZE, &picoSystem);


    //picoTaFileName = (pico_Char *)"C:\\git\\svox\\pico\\lang\\en-GB_ta.bin";
    //picoSgFileName = (pico_Char *)"C:\\git\\svox\\pico\\lang\\en-GB_kh0_sg.bin";

    picoTaFileName = (pico_Char *)"C:\\git\\svox\\pico\\lang\\en-US_ta.bin";
    picoSgFileName = (pico_Char *)"C:\\git\\svox\\pico\\lang\\en-US_lh0_sg.bin";

    picoTaResourceName = (pico_Char *)malloc(PICO_MAX_RESOURCE_NAME_SIZE);
    picoSgResourceName = (pico_Char *)malloc(PICO_MAX_RESOURCE_NAME_SIZE);
    picoUtppResourceName = (pico_Char *)malloc(PICO_MAX_RESOURCE_NAME_SIZE);

    /* Load the text analysis Lingware resource file.   */
    ret = pico_loadResource(picoSystem, picoTaFileName, &picoTaResource);
    if (PICO_OK != ret) {
        printf("failed!");
    }

    /* Load the signal generation Lingware resource file.   */
    ret = pico_loadResource(picoSystem, picoSgFileName, &picoSgResource);
    if (PICO_OK != ret) {
        printf("failed!");
    }

    /* Load the utpp Lingware resource file if exists - NOTE: this file is optional
    and is currently not used. Loading is only attempted for future compatibility.
    If this file is not present the loading will still succeed.                      */
    ret = pico_loadResource(picoSystem, picoUtppFileName, &picoUtppResource);
    if (PICO_OK != ret) {
        printf("failed!");
    }

    /* Get the text analysis resource name.     */
    ret = pico_getResourceName(picoSystem, picoTaResource, (char *)picoTaResourceName);
    if (PICO_OK != ret) {
        printf("failed!");
    }

    /* Get the signal generation resource name. */
    ret = pico_getResourceName(picoSystem, picoSgResource, (char *)picoSgResourceName);
    if ((PICO_OK == ret) && (picoUtppResource != NULL)) {
        /* Get utpp resource name - optional: see note above.   */
        ret = pico_getResourceName(picoSystem, picoUtppResource, (char *)picoUtppResourceName);
        if (PICO_OK != ret) {
            printf("failed!");
        }
    }

    if (PICO_OK != ret) {
        printf("failed!");
    }

    /* Create a voice definition.   */
    ret = pico_createVoiceDefinition(picoSystem, (const pico_Char *)PICO_VOICE_NAME);
    if (PICO_OK != ret) {
        printf("failed!");
    }

    /* Add the text analysis resource to the voice. */
    ret = pico_addResourceToVoiceDefinition(picoSystem, (const pico_Char *)PICO_VOICE_NAME, picoTaResourceName);
    if (PICO_OK != ret) {
        printf("failed!");
    }

    /* Add the signal generation resource to the voice. */
    ret = pico_addResourceToVoiceDefinition(picoSystem, (const pico_Char *)PICO_VOICE_NAME, picoSgResourceName);
    if ((PICO_OK == ret) && (picoUtppResource != NULL)) {
        /* Add utpp resource to voice - optional: see note above.   */
        ret = pico_addResourceToVoiceDefinition(picoSystem, (const pico_Char *)PICO_VOICE_NAME, picoUtppResourceName);
        if (PICO_OK != ret) {
            printf("failed!");
        }
    }

    if (PICO_OK != ret) {
        printf("failed!");
    }

    ret = pico_newEngine(picoSystem, (const pico_Char *)PICO_VOICE_NAME, &picoEngine);
    if (PICO_OK != ret) {
        printf("failed!");
    }


    FILE *fp = fopen("output.wav", "wb");

    const int MAX_OUTBUF_SIZE = 1024;
    short       outbuf[MAX_OUTBUF_SIZE / 2];
    pico_Char* inp = (pico_Char *)"this is a test!";;

    int text_remaining = strlen((const char *)inp) + 1;
    pico_Int16 bytes_sent;
    pico_Int16 bytes_recv;
    pico_Int16 out_data_type;

    while (text_remaining) {
        ret = pico_putTextUtf8(picoEngine, inp, text_remaining, &bytes_sent);
        if (PICO_OK != ret) {
            printf("failed!");
        }

        text_remaining -= bytes_sent;
        inp += bytes_sent;

        do {
            /* Retrieve the samples and add them to the buffer. */
            ret = pico_getData(picoEngine, (void *)outbuf, MAX_OUTBUF_SIZE, &bytes_recv,
                &out_data_type);
            if (bytes_recv) {
                printf("got %d\n", bytes_recv);
                fwrite(outbuf, 1, bytes_recv, fp);
            }
        } while (PICO_STEP_BUSY == ret);
    }


    fclose(fp);


    cleanResources();


    if (picoSystem) {
        pico_terminate(&picoSystem);
        picoSystem = NULL;
    }
    if (picoMemArea) {
        free(picoMemArea);
        picoMemArea = NULL;
    }
}

