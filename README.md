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


## Configuração do Ambiente

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





