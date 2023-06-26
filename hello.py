import requests

def fetch_html_page(url):
    # try:
        response = requests.get("https://" +url)
        # response.raise_for_status()  # Raise an exception for 4xx and 5xx status codes
        return response.text
    # except requests.exceptions.RequestException as e:
        # print("An error occurred:", e)
        # return None

# Example usage
url = input("Enter the URL: ")
html_content = fetch_html_page(url)
if html_content:
    print(html_content)
