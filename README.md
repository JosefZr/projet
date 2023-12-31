# projet
projet système d'exploitation

On veut effectuer en parallèle(En utilisant le modèle producteurs/consommateur) le produit de deux matrices: 
B (n1* m1)  et C (n2 * m2) ⇒ la matrice résultante A=B*C ;

Les matrices sont remplis par des valeurs aléatoires

Les résultats intermédiaires seront placés dans un tampon de taille “T[N]”.

Chaque threads producteurs calcule une ligne de la matrice résultante A et range les résultat dans le tampon T

Les threads consommateurs consomment l'élément T[y]  le place dans la matrice résultante A  au bon emplacement!

q1: Quelles sont les structures de données à utiliser ?

q2: Comment allez-vous protéger l'accès à ces données?

q3- quels sont les risques?

1-Cloner le projet github : projet  ; et le modifier le selon les exigences ci-dessus

2- Pour chaque nouvelle idée créer une nouvelle branche; les autres étudiants peuvent améliorer l'idée en créant une nouvelle branche!

3-Les premières réponses sont mieux notées!

4-Bien gérer les éxceptions 

5-Bien gérer les messages d'erreurs!


q1: Quelles sont les structures de données à utiliser ?

->  Matrices B, C, et A : pour stocker les données des matrices.
    Tampon T : pour stocker les résultats intermédiaires.
   
    
q2: Comment allez-vous protéger l'accès à ces données?

->  Nous utiliserons un mutex pour protéger l'accès aux données partagées, notamment aux matrices et au tampon T. Avant de lire ou de modifier une donnée partagée, chaque thread devra acquérir le mutex, et une fois l'opération terminée, il devra libérer le mutex.

Nous utiliserons également des sémaphores (empty et full) pour synchroniser les producteurs et les consommateurs, en indiquant si le tampon est vide ou plein.

 q3 : Quels sont les risques?
 
 -> Les principaux risques comprennent les conditions de concurrence (race conditions) lors de l'accès aux données partagées. Cela peut se produire si plusieurs threads essaient de modifier les mêmes données simultanément sans synchronisation appropriée. Les risques potentiels incluent la corruption des données et des résultats incorrects.
