def parse(text):
    initial_asterisk = text.find("***")
    header_cutoff = text.find("\n", initial_asterisk)
    footer_cutoff = text.find("***", header_cutoff + 1)
    paragraphs = text[header_cutoff:footer_cutoff].split('\n\n')
    return [i for i in paragraphs if i.strip()]
