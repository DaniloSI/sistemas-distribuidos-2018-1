Questões
=========


Capítulo 1
----------

1) Compare as definições de Sistema Distribuído feitas por Tanenbaum e por Coulouris (ver slides) e crie uma definição própria que contemple ambas.
2) Qual o papel do Middleware em Sistemas Distribuídos?
3) Quais são as metas de um Sistema Distribuído?
4) Duas motivações para a utilização de um Sistema Distribuído são (i) economia e (ii) colaboração (troca de informações). Cite 3 exemplos de cada uma.
5) Descreva brevemente os tipos de transparência. Dê exemplos.
6) Qual a diferença entre migração e relocação?
7) Qual a transparência mais difícil de ser implementada e porque?
8) Por que nem sempre é uma boa ideia visar à implementação do mais alto grau de transparência possível?
9) O que é um sistema distribuído aberto e quais os benefícios que a abertura proporciona?
10) Diferencie Interoperabilidade e Portabilidade.
11) Com relação a Escalabilidade de um sistema distribuído:
	a. Descreva 3 dimensões (medida de escalabilidade);
	b. Cite 3 exemplos de limitações (problemas de escalabilidade);
	c. Cite 3 técnicas (de escalabilidade).
12) Aponte os desafios (ciladas) que tornam a construção de um Sistema Distribuído mais complexa que a de um sistema convencional.
13) Qual a diferença entre a computação Cluster e a Grid?
14) Quais as propriedades de uma transação?
15) Explique o funcionamento de uma transação aninhada.
16) O que é um monitor de processamento de transações (monitor TP)?
17) Dê exemplos de sistemas pervasivos.


Capítulo 2
----------

18) No contexto de um sistema distribuído, defina o que são:
	a. Componentes de software;
	b. Estilos arquitetônicos;
	c. Arquitetura de sistemas.
19) Descreva a organização dos estilos arquitetônicos abaixo:
	a. Arquitetura em camadas;
	b. Arquitetura baseada em objetos;
	c. Arquitetura centrada em dados;
	d. Arquitetura baseada em eventos.
20) Com relação a arquitetura de sistema, existem 3 tipos principais (ver abaixo). Descreva a organização de cada um delas:
	a. Centralizadas;
	b. Descentralizadas;
	c. Híbridas.
21) Observe a Figura 2.5 (a)-(e) do livro texto (Tanembaum, S.D., 2a ed.).
	a. Dê exemplos de sistemas para as alternativas de organizações de cliente-servidor apresentadas nos itens (a), (c) e (e).
	b. Defina (i) clientes gordos (fat clients) e (ii) clientes magros (thin clients), e indique em que alternativa estaria cada um.
	c. Qual das alternativas é mais dependente da plataforma do cliente? E por que?
22) O que é uma arquitetura cliente-servidor de três divisões?
23) Qual a diferença entre uma distribuição vertical e distribuição horizontal?
24) Procure na Web e ilustre com exemplos reais sistemas com as seguintes arquiteturas:
 a. Cliente-Servidor;
 b. Peer-to-Peer;
 c. Proxy Server;
 d. Código Móvel;
 e. Cliente Magro (Thin Client);
 f. Agente Móvel;
 g. N-Camadas ?


Respostas
=========

1) 
2) O papel do Middleware em Sistemas Distribuídos é fornecer transparência de distribuição de recursos, abstrair a heterogeneidade dos componentes do Sistema Distribuído e oferecer uma visão de sistema único.
3) As metas de um Sistema Distribuído são (i) ocultar distribuição de recursos, (ii) ser aberto e (iii) poder ser expandido.
4) (i) Exemplos de motivações para utilização de um Sistema Distribuído, relacionadas à economia:
	a) 