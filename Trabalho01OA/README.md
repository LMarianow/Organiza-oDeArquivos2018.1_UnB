# UNB / Organização de Arquivos - 2018/01
### Trabalho 01 Indexação de Arquivos

##### Professor 
- Camilo Dórea

##### Alunos
- Lucas Mariano Carvalho 16/0133661
- Heitor de Lima Belém   16/0123950

##### Objetivos
- Gerar Índices Primários e Secundários
- Inclusão de novos Registros
- Exclusão de Registros
- Ordenação dos Registros e dos índices
- Atualização de Registros
- Intercalação (merge) das duas listas.

##### Listas
- Lista 1
  
  ```
024312 Leonardo Nunes de Souza                  32  EC       A
031272 Andreza Pollyana da Silva                32  EC       A
035830 Rogerio Augusto de Castro Neves          37  EC       A
041881 Ana Luisa Costa Miguel                   34  CC       A
042290 Bruno Filipe Milare de Macedo            34  EC       A
042780 Daniel Schowantz Silva                   34  EC       A
042865 Danilo Silva Marshall                    35  CC       A
043217 Erikita Regina Campos de Almeida         34  CC       A
043341 Fabricio Nallin Violin                   34  EC       A
044072 Igor Ribeiro de Assis                    34  EC       A
045058 Marcio Alexandre Marar Moreno            34  EC       A
046306 Rodrigo Dias Takase                      35  EC       A
046708 Thiago de Moraes Sfredo                  35  EM       A
046713 Thiago Fernandes Crepaldi                34  EC       A
046802 Tiago Rezende Campos Falcao              34  EC       A
046944 Vitor Baccetti Garcia                    34  EC       A
046946 Vitor Bruno Jacintho de Almeida          34  EC       A
046956 Vitor Gouvea Franchi                     34  EC       A
046961 Vitor Paulo Villarino Pinto              32  EC       A

  ```
  
- Lista 2 
  
  ```
015511 Angelo Luiz Rocatto Junior               34  CC       B
031147 Anderson Silva Bezerra                   34  EC       B
031200 Andre Luiz Silva Bezerra                 34  CC       B
032725 Fellipe Choi                             32  CC       B
033282 Henrique Elias da Silva                  34  CC       B
037396 Hugo Torres de Carvalho Brandao          34  CC       B
042348 Bruno Teles                              34  EC       B
042728 Daniel Alonso Negrini                    34  EC       B
043648 Franz Glauber Vanderlinde                38  EC       B
044281 Jorge Luiz Silverio dos Santos           34  CC       B
044637 Leonardo Machado Carreiro                34  CC       B
045460 Murilo Bianchini                         34  EC       B
044072 Igor Ribeiro de Assis                    34  CC       B
045585 Otavio Monteiro Becker Neto              34  EC       B
045927 Rafael Obregon de Melo                   34  CC       B
042780 Daniel Schowantz Silva                   34  EM       B
045934 Rafael Pinhate Bandeira                  34  CC       B
046245 Roberto Rodrigues Borgognoni             35  CC       B
046674 Tharsis Tuani Pinto Souza                37  EM       B
046718 Thiago Henrique Parra                    35  EC       B
046961 Vitor Paulo Villarino Pinto              34  CC       B
024312 Leonardo Nunes de Souza                  34  CC       B

  ```

##### Execução do Trabalho 01
   * clonar esse repositório via o git 
   * criar o banco de dados imdb no mysql 
   * definir o esquema do banco de dados (verificar o script em conf/evolutions/1.sql)
   * executar o sbt (isso no diretório raiz do projeto) 
   * no shell do sbt, executar "run" 
   * acessar o projeto em http://localhost:9000  
- Clonar o repositório via git e acessá-lo utilizando
  ```
  git clone https://github.com/lmarianow/OrganizaçãoDeArquivos2018.1_UnB.git
  ```
  ```
  cd OrganizaçãoDeArquivos2018.1_UnB
  ```
  ```
  cd Trabalho01OA
  ```
- Verificar se a versão instalada do G++ é >= ``` g++ (Ubuntu 5.4.0-6ubuntu1~16.04.9) ```

- Digitar os seguintes comandos no terminal:
  ```
  g++ -o Trabalho01 Trabalho01.c
  ```

  ```
  ./Trabalho01
  ```

##### Exemplo de saída
- Apenas uma imagem de exemplo de execução do projeto.

Menu de opções:
<p align="center">
<img src="https://i.imgur.com/VwmURSw.png">
</p>
