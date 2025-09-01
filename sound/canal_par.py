import re

num=1
input_file = f"Bass_Drum_{num}.c"
output_file = f"Bass_Drum_{num}_channel1.c"

# Ler o arquivo original
with open(input_file, "r") as f:
    content = f.read()

# Encontrar o array com regex
# Isso captura tudo entre { e };
array_pattern = re.compile(r"(uint16_t\s+\w+\s*\[\s*\]\s*=\s*\{)(.*?)(\};)", re.DOTALL)
match = array_pattern.search(content)

if not match:
    raise ValueError("Array não encontrado no arquivo")

header, array_content, footer = match.groups()

# Extrair os números
numbers = [int(n) for n in re.findall(r"-?\d+", array_content)]

# Pegar apenas os índices pares (canal 1)
channel1 = numbers[::2]
channel1 = [int((n + 32768) * 127 / 65535) for n in channel1] 

# Gerar o novo conteúdo do array como string, mantendo a formatação
new_array_content = ""
for i, n in enumerate(channel1):
    # Dois valores por linha é opcional, aqui colocamos 2 por linha para legibilidade
    if i % 2 == 0 and i != 0:
        new_array_content += "\n"
    new_array_content += f"{n}, "

# Montar o novo conteúdo do arquivo
new_content = content[:match.start(2)] + new_array_content + content[match.end(2):]


# Salvar em um novo arquivo
with open(output_file, "w") as f:
    f.write(new_content)

print(f"Arquivo gerado: {output_file} com {len(channel1)} elementos do canal 1")

