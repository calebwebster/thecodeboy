#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  return size * nmemb;
}

CURLcode ntfy(CURL *curl, char *channel_name, char *message) {
  char url[15 + strlen(channel_name) + 1];
  int j = snprintf(url, sizeof(url), "http://ntfy.sh/%s", channel_name);

  CURLcode res = CURLE_OK;

  res = curl_easy_setopt(curl, CURLOPT_URL, url);
  if (res != CURLE_OK)
    return res;

  res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
  if (res != CURLE_OK)
    return res;

  res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(message));
  if (res != CURLE_OK)
    return res;

  res = curl_easy_perform(curl);
  if (res != CURLE_OK)
    return res;

  return res;
}

int main(void) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Curl failed to initialize");
    return 1;
  }

  CURLcode res = CURLE_OK;
  res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_setopt() failed: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return 1;
  }

  static char *channel_name = "test";
  static char *message = "The Code Boy is mid as hell: "
                         "\nhttps://open.spotify.com/artist/"
                         "21BMl27VlpO43ZudKAl0L8?si=2k5IEhEQTw6VB-IYuhKlsQ";

  while (1) {
    res = ntfy(curl, channel_name, message);
    if (res != CURLE_OK) {
      fprintf(stderr, "ntfy failed: %s\n", curl_easy_strerror(res));

      sleep(30);
    } else {
      time_t rawtime;
      struct tm *info;
      char buffer[128];
      time(&rawtime);
      info = localtime(&rawtime);
      strftime(buffer, 128, "%a %b %d %X", info);

      printf("%s Successfully notified everyone that The Code Boy is mid "
             "as hell\n",
             buffer);

      sleep(30 * 60);
    }
  }

  curl_easy_cleanup(curl);
  return 0;
}
