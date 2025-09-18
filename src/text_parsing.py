def parse(text):
    initial_asterisk = text.find("***")
    header_cutoff = text.find("\n", initial_asterisk)
    print(header_cutoff)
    #footer_asterisk = text.find("***", header_cutoff)
    footer_cutoff = text.find("***", header_cutoff + 1)
    print(footer_cutoff)

    paragraphs = text[header_cutoff:footer_cutoff].split('\n\n')
    return [i for i in paragraphs if i.strip()]
    #return [i.strip() != "" for i in paragraphs]
