#pragma once
#include <systemc.h>
#include "debug_util.h"

/*
** Le composant "fifo" est une file "first-in first-out", c'est à dire que le premier élément
** est aussi le premier élément qui sort (comme une queue de supermarché, en quelque sorte)
** Dans la version actuelle, les "fifo" ne font qu'une place, c'est à dire qu'elles ressemblent
** plus à des registres au fonctionnement un peu particulier.
**
** Signaux d'entrée
** - PUSH qui permet d'écrire une donnée dans le registre interne de la fifo
** - POP qui permet de sortir la donnée du registre interne de la fifo
** - DATAIN qui est la donnée d'entrée
**
** Signaux de sortie
** - FULL qui signale que la fifo est pleine et qu'il est impossible d'y envoyer une nouvelle donnée
** - EMPTY qui signale que la fifo est vide est qu'il est impossible d'y lire une donnée
** - DATAOUT, la donnée de sortie.
**
** Ordre des opérations par rapport au cycles d'horloge
** - Pour le PUSH : l'étage entrant met la donnée qu'il souhaite mettre dans la fifo
**   dans DATAOUT, et le signal PUSH à 1. La commande sera pris en compte au prochain
**   front montant, et si la fifo n'est pas pleine (FULL), la donnée sera écrite dans
**   son registre interne
**
** - Pour le POP : la donnée interne est toujours (quelle que soit la valeur du signal POP)
**   mise à disposition dans DATAOUT. Lorsqu'il en a besoins, l'étage sortant l'utilise, et
**   met le signal POP à 1 pour signifier qu'il l'a utilisé. La commande sera pris en compte
**   en fin de cycle, et la donnée interne sera soit invalidée (au quel cas la fifo deviendra
*EMPTY),
**   soit remplacée par une donnée qui vient d'être PUSH.
**
** Note : Lorsque la fifo contient une donnée, il est possible de PUSH et de POP au même cycle.
**   Lorsque la fifo ne contient pas de donnée, c'est impossible, pour préserver au moins un cycle
*de
**   délai entre l'entrée et la sortie de la fifo.
**   Ainsi, la fifo est EMPTY (=il est impossible d'y prendre une donnée) dés lors qu'elle ne
*contient
**   pas de données.
**   En revanche, la fifo est FULL (=il est impossible d'y placer une donnée) si elle contient déjà
*une
**   donnée *et* que le signal POP vaut 0, c'est à dire que la donnée actuellement présente restera
*au cycle
**   suivant et ne sera pas consommée.
**
*/
template <int T>
SC_MODULE(fifo) {
    // signaux de contrôle, clock et reset
    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // signaux d'entrée
    sc_in<sc_bv<T>> DATAIN_S;
    sc_in<bool>     PUSH_S;
    sc_in<bool>     POP_S;

    // signaux de sortie
    sc_out<bool> FULL_S;
    sc_out<bool> EMPTY_S;
    // DATAOUT_R est à la fois le signal de sortie, et le registre interne de la fifo
    sc_out<sc_bv<T>> DATAOUT_R;

    // bit de validité du registre interne de la fifo
    sc_signal<bool> valid;

    void clock_update();
    void flags_update();
    void trace(sc_trace_file * tf);
    SC_CTOR(fifo) {
        SC_CTHREAD(clock_update, fifo::CLK.pos());
        SC_METHOD(flags_update);
        sensitive << PUSH_S << POP_S << valid << DATAOUT_R;

        reset_signal_is(RESET, false);
    }
};
template <int T>
void fifo<T>::flags_update() {
    // cas où la donnée dans la fifo est valide
    if (valid) {
        // dans ce cas, la fifo n'est jamais vide
        EMPTY_S.write(0);
        // si on POP, n'est pas pleine non plus, cas on pourra y ajouter une donnée
        // si on ne POP pas, la fifo est pleine
        if (POP_S)
            FULL_S.write(0);
        else
            FULL_S.write(1);
    } else  // cas ou la donnée dans la fifo est invalide
    {
        // Dans ce cas, la fifo ne peut pas être pleine, et elle est toujours vide
        FULL_S.write(0);
        EMPTY_S.write(1);
    }
}

template <int T>
void fifo<T>::clock_update() {
    // code de reset de la fifo : la donnée interne vaut 0 et n'est pas valide
    valid     = false;
    DATAOUT_R = 0;

    wait(1);

    while (1) {
        bool push = PUSH_S.read();
        bool pop  = POP_S.read();
        // Si on POP sans PUSH, alors la fifo se vide, et la donnée devient invalide
        if (!push && pop) valid = false;
        // Au contraire, si on push et que la FIFO n'est pas pleine, on écrit l'entrée dans
        // le registre, et on note que la donnée est valide
        else if (push && !FULL_S) {
            DATAOUT_R.write(DATAIN_S.read());
            valid = true;
        }
        wait(1);
    }
}

// fonction qui permet de tracer les signaux de la fifo
template <int T>
void fifo<T>::trace(sc_trace_file* tf) {
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, DATAIN_S, GET_NAME(DATAIN_S));
    sc_trace(tf, PUSH_S, GET_NAME(PUSH_S));
    sc_trace(tf, POP_S, GET_NAME(POP_S));
    sc_trace(tf, FULL_S, GET_NAME(FULL_S));
    sc_trace(tf, EMPTY_S, GET_NAME(EMPTY_S));
    sc_trace(tf, DATAOUT_R, GET_NAME(DATAOUT_R));
    sc_trace(tf, valid, GET_NAME(valid));
}