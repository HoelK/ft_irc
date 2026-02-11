Client et Channel sont juste des classes avec des setters et getters (+ quelques utilitaires) pour faire fonctionner les commandes correctement.

Server fait juste tourner le système de sockets.

MSG | CMD | RPL sont 3 classes qui communiquent à travers la structure Packet.

Leurs 3 fonctions principales sont respectivement sendData, apply, reply.
- MSG parse le message reçu et remplit les données de base du packet.
- CMD fait les modifications sur les données internes et ajoute des informations au packet (erreurs et statut de la commande).
- RPL envoie la réponse au(x) client(s) concerné(s).

CMD et RPL appellent leurs fonctions à travers une map avec pour clé la commande (packet.cmd).

Lorsqu'on ajoute une commande, il faut créer sa fonction, créer la macro qui représente sa commande (CMD.hpp), puis l'ajouter à la map de CMD et RPL.

Lorsqu'un cas d'erreur est rencontré dans une commande (CMD), la variable packet.error est définie avec la valeur appropriée (définie par le protocole RFC 1459).
Reply s'occupera ensuite d'envoyer le message d'erreur au client concerné.

Il faut ajouter la commande invite, mode (les operateur de channels) et fix les edges cases et cas d'erreurs non gerer
