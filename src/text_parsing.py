from collections import Counter
import re

def chunk(text):
    initial_asterisk = text.find("***")
    header_cutoff = text.find("\n", initial_asterisk)
    footer_cutoff = text.find("***", header_cutoff + 1)
    paragraphs = [char for char in (text[header_cutoff:footer_cutoff].split('\n\n')) if char.strip() != ''] # we don't even use this
    scentences = (text[header_cutoff:footer_cutoff].split('.')) # inaccurate way of splitting strings
    words = re.findall(r'\b\w+\b', text[header_cutoff:footer_cutoff].lower()) # regex to get words
    return (paragraphs, scentences, words)

def parse(text):
    paragraphs, scentences, words = chunk(text)
    count = Counter(words)
    avg_scentence_len = len(words) / len(scentences)
    word_order = count.most_common()
    print(f"NAME \\ {word_order[0][0]}"\
            " {word_order[1][0]}"\
            " {word_order[2][0]}"\
            " {word_order[3][0]}"\
            " {word_order[4][0]}"\
            " \\ {avg_scentence_len}"
          )

