import text_parsing

book_file = open("../data/book.txt", 'r', encoding='utf-8')

print(text_parsing.parse(book_file.read()))

book_file.close()
