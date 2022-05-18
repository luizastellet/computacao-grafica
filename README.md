# Transformações 2D 

## Matriz Identidadde

		| 1  0  0 |
	    M = | 0  1  0 |
		| 0  0  1 |

## Função operaTransforma()
Esta função é chamada por todas as transformações, pois é ela quem realmente cálculo da nova posição do objeto. 

Criamos o vetor auxiliar **temp[3]** para armazenar os resultados das operações, e as variáveis i e j para percorrer a matriz e os vetores. 

        float temp[3];
        int i, j;


As linhas 1 e 3 são responáveis por percorrer a matriz e o vetores. 

A linha 2 "reinicializa" a posição **i** do vetor **temp**,  garantindo que nâo haja nenhum lixo ali. 

Na linha 4 é onde a operação de fato acontece. Multiplica-se a matriz de operação **gMatriz** pelos valores (x,y) do vértice em questão e armazena-se o resultado em **temp**. 

    1   for(i = 0; i < 3; i++) {
    2          temp[i] = 0.0f;
    3           for(j=0; j<3; j++){
    4                temp[i] = temp[i] + gMatriz[i][j] * pvertex[position].v[j];
    5            }
    6        }
**Obs**: A variável **position** é recebida como parâmetro e representa a linha da matriz de vértices **pvertex** que está sendo operada. 

Por último, atualizamos o novo valor do vetor na matriz de vértice **pvertex**. 

        for( i = 0; i < 3; i++){
                pvertex[position].v[i] = temp[i];
            }	
## Transladar objeto à origem do sistema
Em algumas transformações, precisaremos trazer o objeto à origem do sistema. Para tal, precisamos encontrar o centro do objeto e depois realizar a translação do mesmo. 

### calCentro()
Função que calcula o centro do polígono. 

1. Somar vértices do coordenada a coordenada.

        for (i=0; i<gNumVert; i++) {
            cc[0] += pvertex[i].v[0];
            cc[1] += pvertex[i].v[1];
            cc[2] += pvertex[i].v[2];
        }

2.  Dividir o resultado da soma pelo número de vértices.

        cc[0] /= gNumVert;     
        cc[1] /= gNumVert;     
        cc[2] /= gNumVert; 

### translate() 
Ao chamar esta função, passamos como argumento o centróide que calculamos acima.

Para trazer o polígono à origem, multiplicamos os valores do centróide por -1. 
        translate(-1*vc[0], -1*vc[1]);   

Assim, a matriz de translação ficará: 

			| 1   0  -dx ||x|   |x - dx|
		    M = | 0   1  -dy ||y| = |y - dy|
			| 0   0   1  ||1|   |  1   |

Agora já é possível realizar a transformação deseja, como, por exemplo, a rotação. 

Para devolver o polígono à posição inicial, chamaremos a função novamente, porém, desta vez, **não** multplicaremos por -1.  
        translate(vc[0], vc[1]);    

## Translação 
Transladar é movimentar um objeto. É possível realizar a translação de um objeto adicionando quantidades às suas coordenadas (x,y).

Entâo, a matriz de translação é dada por: 

			| 1  0  dx ||x|   |x + dx|
		    M = | 0  1  dy ||y| = |y + dy|
			| 0  0  1  ||1|   |  1   |


Para realizar a translação:
 1. Gerar matriz identidade;

        matrizIdentidade(); 

 2. Inserir dx e dy na matriz identidade; 
      
        gMatriz[0][2] = dx;
        gMatriz[1][2] = dy;

3. Operar a transformação em cada um dos vértices. 

        for (int i = 0; i < gNumVert; i++) {
		    operaTransforma(i);
	    }

## Rotação 
Rotacionar significa girar, alterar a orientação de um objeto. Para que isso seja feito, se faz necessária uma combinação de translação em direção à origem do sistema e rotação. 


A matriz de rotação é dada por: 

			| cos(α)  -sin(α)   0 ||x|   |x.cos(α) - y.sin(α)|
		    M = | sin(α)   cos(α)   0 ||y| = |y.cos(α) + x.sin(α)|
			|   0        0      1 ||1|   |         1         |


1. Calcular centro do objeto e transladá-lo à origem do sistema. 

            calCentro(vc);                    
            translate(-1*vc[0], -1*vc[1]);

2. Determinar ângulo da rotação através do produto vetorial (v[0], [1]) entre vetorial original e final (dx, dy). 

        oo = pvertex[gIndVert].v[1] * dx - pvertex[gIndVert].v[0] * dy;


3. Determinar orientação da rotação

        teta = gAng;                
        if(oo > 0.0f) {
            teta = -1.0f * gAng; 		 
	    }
    Se oo > 0, então a rotação é anti-horária. 
    Se oo < 0, então a rotação é horária. 

4. Gerar matriz identidade;

        matrizIdentidade(); 

5. Definir matriz de rotação; 
      
        	 gMatriz[0][0] = cos(teta);
            gMatriz[0][1] = -sin(teta);
 	        gMatriz[1][0] = sin(teta);
            gMatriz[1][1] = cos(teta);
 	 
6. Operar a transformação em cada um dos vértices. 

        for (int i = 0; i < gNumVert; i++) {
		    operaTransforma(i);
	    }

7. Transladar objeto de volta à sua posição original. 

        translate(vc[0], vc[1]);

## Escala 
A transformação de escala consiste da alteração do tamanho do modelo. Esta tranformação, assim como a de rotação, também é composta por duas operações: translação e mudança de escala. 

A matriz de escala é dada por: 

			| Sx  0   0 ||x|   |x.Sx|
		    M = | 0   Sy  0 ||y| = |y.Sy|
			| 0   0   1 ||1|   | 1  |

1. Calcular centro do objeto e transladá-lo à origem do sistema. 

        calCentro(vc);                    
        translate(-1*vc[0], -1*vc[1]);

2. Determinar os fatores de escala em relação aos eixos X e Y. 

        Sx = Sy = 1.0f;
        if(fabs(pvertex[gIndVert].v[0]) > 0.01f){
		    Sx = 1.0f + dx / pvertex[gIndVert].v[0];
	    }
		
        if(fabs(pvertex[gIndVert].v[1]) > 0.01f){
		    Sy = 1.0f + dy / pvertex[gIndVert].v[1];
    	}


3. Gerar matriz identidade;

        matrizIdentidade(); 

4. Definir matriz de escala; 
      
        gMatriz[0][0] = Sx;
 	    gMatriz[1][1] = Sy;
 	 
5. Operar a transformação em cada um dos vértices. 

        for (int i = 0; i < gNumVert; i++){
		    operaTransforma(i);
	    }

6. Transladar objeto de volta à sua posição original. 

        translate(vc[0], vc[1]);

## Cisalhamento


## Reflexão 

