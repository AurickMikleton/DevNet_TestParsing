from collections import Counter

def paragraph_chunk(text):
    initial_asterisk = text.find("***")
    header_cutoff = text.find("\n", initial_asterisk)
    print(header_cutoff)
    footer_cutoff = text.find("***", header_cutoff + 1)
    print(footer_cutoff)

    return [char for char in (text[header_cutoff:footer_cutoff].split('\n\n')) if char.strip() != '']

def parse(text):
    paragraphs = paragraph_chunk(text)
    #print(paragraphs)
    total_words = []
    for i in paragraphs:
        words = i.split()
        total_words += words;
    count = Counter(total_words)
    print(count)

