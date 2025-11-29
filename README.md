# Spiderman-game-allegro

# Configurações iniciais

Este é um jogo simples do Homem-Aranha desenvolvido usando a biblioteca Allegro. Siga as instruções abaixo para configurar o ambiente de desenvolvimento e executar o jogo.

## Requisitos

- Sistema Operacional: Windows, Linux ou macOs
- Compilador C/C++ (GCC, Clang, MSVC)
- Biblioteca Allegro 5 instalada em seu sistema
- CMake (opcional, mas recomendado)
- Git (para trabalhar no repositório)
- Editor de código ou IDE (Visual Studio, Visual Studio Code, Code ::Blocks, etc.)
- Ferramenta de construção (Make, Ninja, etc.)
- Bibliotecas adicionais (se necessário, como SDL2, OpenGL, etc.)

# Compilação com GCC

1. **Instalar Biblioteca Allegro**
   - Entre no seguinte link dentro do GitHub para instalar a biblioteca: https://github.com/liballeg/allegro5
   - Vá até “Releases” no canto direito da tela -> role o scroll do mouse até achar “Assets” -> procure pelo arquivo allegro-x86_64-w64-mingw32-gcc-15.2.0-posix-seh-static-5. para baixar
   - Descompacte o arquivo.zip e salve a pasta allegro no Disco Local C.

2. **Configurar Projeot**
   - Baixe o arquivo.zip no nosso repositório do GitHub ou no AVA
   - Acesse a pasta que está localizado o projeto dentro do Prompt de Comando (Cmd). Ex: cd Doc; cd Projeto
   - Após isso, digite o seguinte comando: “gcc -I C:\allegro\include -c main.c”
   - Desta forma será criado o arquivo main.o. Digite o comando “dir” no cmd para verificar se o arquivo foi criado
   - Após isso, copie os arquivos “liballegro_monolith.dll.a” dentro de alegro\lib e “allegro_monolith-5.2.dll” dentro de alegro\bin e coloque dentro da pasta do projeto
   - Ou se preferir utilize o seguinte comando no próprio cmd para adicionar os arquivos: “xcopy C:\allegro\bin\ allegro_monolith-5.2.dll” e “xcopy C:\allegro\lib\ liballegro_monolith.dll.a”
   	*Obs: Pode substituir os arquivos que já estão na pasta caso peça.
   - Agora, utilize o seguinte comando para criar um arquivo.exe do projeto: “gcc -I C:\allegro\include main.o -o program.exe liballegro_monolith.dll.a”
   - Por fim, basta rodar o jogo com o comando no cmd: “program.exe”

## Configuração do Ambiente (Microsoft Visual Studio)

1. **Instalar a Biblioteca Allegro**
   - Siga as instruções oficiais da [documentação do Allegro](https://liballeg.org/) para instalar a biblioteca em seu sistema operacional.
	- Faça a instalação usando o gerenciador de pacotes do seu sistema.
	- Caso esteja usando o Visual Studio, faça a instalação no gerenciador de pacotes NuGet, pesquisando por "allegro".

2. **Clonar o Repositório**
	- Clone o [repositório](https://github.com/JoaoPedroDevOz/Spiderman-game-allegro) do jogo usando o Git.
	- Crie uma branch para trabalhar no projeto.
	- Teste suas alterações localmente.
	- Faça o commit e depois o merge request para a branch principal.

3. **Configurar o Projeto**
	- Verificar se está compilando corretamente com a Allegro.
	- Caso ocorra erros na compilação: Desinstale o Allegro e instale novamente no NuGet, em modo de Debug, compile o código novamente.
	- Em última instância, siga os passos a seguir:

	- Em propriedades do projeto (apertando com o botão direito no projeto: "Spiderman-game-allegro" > Propriedades):
		- Adicione os diretórios de inclusão do Allegro em "C/C++" > "Geral" > "Diretórios de Inclusão Adicionais": $(ProjectDir)packages\Allegro.5.2.10\build\native\include
		- Adicione os diretórios de bibliotecas do Allegro em "Vinculador" > "Geral" > "Diretórios de Bibliotecas Adicionais": $(ProjectDir)packages\Allegro.5.2.10\build\native\v143\x64\lib
		- Adicione as bibliotecas necessárias em "Vinculador" > "Entrada" > "Dependências Adicionais":
			- allegro_monolith-static
			- opengl32.lib
			- winmm.lib
			- shlwapi.lib

			- Caso prefira pegar modulos específicos da allegro, adicione os seguintes modulos, ao invés de allegro_monolith-static:
			- allegro.lib
			- allegro_font.lib
			- allegro_primitives.lib
			- allegro_ttf.lib
			- allegro_image.lib
			- allegro_dialog.lib
			- allegro_main.lib
  


   
