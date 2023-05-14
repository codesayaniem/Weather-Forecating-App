#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cJSON.h>

// Function to handle CURL write callback
size_t write_callback(char *data, size_t size, size_t nmemb, char *buffer) {
  // Calculate the total size of the data
  size_t total_size = size * nmemb;

  // Reallocate the buffer if necessary
  buffer = realloc(buffer, total_size + 1);
  if (buffer == NULL) {
    // Print an error message and return 0
    printf("Error: Unable to allocate memory.\n");
    return 0;
  }

  // Append the data to the buffer
  strncat(buffer, data, total_size);

  // Return the number of bytes written
  return total_size;
}

// This is the main function
int main() {
  // Create a CURL handle
  CURL *curl;

  // Initialize the CURLcode variable
  CURLcode res;

  // The URL of the weather API
  char *weather_api_url = "API_URL_HERE";

  // Your API key
  char *api_key = "YOUR_API_KEY_HERE";

  // The buffer to store the response data
  char *buffer = malloc(4096 * sizeof(char));
  if (buffer == NULL) {
    // Print an error message and return 1
    printf("Error: Unable to allocate memory.\n");
    return 1;
  }

  // Initialize the buffer to empty
  buffer[0] = '\0';

  // Create a CURL handle
  curl = curl_easy_init();
  if (curl == NULL) {
    // Print an error message and return 1
    printf("Error: Unable to initialize CURL.\n");
    return 1;
  }

  // Set the CURLOPT_URL option
  curl_easy_setopt(curl, CURLOPT_URL, weather_api_url);

  // Set the CURLOPT_WRITEFUNCTION option
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

  // Set the CURLOPT_WRITEDATA option
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);

  // Perform the request
  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    // Print an error message and return 1
    printf("Error: %s\n", curl_easy_strerror(res));
    return 1;
  }

  // Clean up the CURL handle
  curl_easy_cleanup(curl);

  // Parse the JSON response
  cJSON *json = cJSON_Parse(buffer);
  if (json == NULL) {
    // Print an error message and return 1
    printf("Error: Failed to parse JSON.\n");
    free(buffer);
    return 1;
  }

  // Extract the desired weather data from JSON
  cJSON *temperature = cJSON_GetObjectItem(json, "temperature");
  cJSON *humidity = cJSON_GetObjectItem(json, "humidity");
  cJSON *description = cJSON_GetObjectItem(json, "description");

  // Display the weather data
  printf("Temperature: %.2fÂ°C\n", temperature->valuedouble);
  printf("Humidity: %.2f%%\n", humidity->valuedouble);
  printf("Description: %s\n", description->valuestring);

  // Clean up the JSON data
  cJSON_Delete(json);

  // Free the buffer
  free(buffer);

  // Return 0
  return 0;
}