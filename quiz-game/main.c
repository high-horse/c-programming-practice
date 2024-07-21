#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUESTIONS 10
#define ANSWER_LENGTH 100 

struct Qn_set {
    char *question;
    char *correct_answer;
    char user_answer[ANSWER_LENGTH];
};

struct Answer {
    char user_answer[ANSWER_LENGTH];
};

void generate_qn_set(struct Qn_set qn_set[]) {
    char *questions[QUESTIONS] = {
        "What is the capital city of France? \n",
        "Who wrote the play 'Romeo and Juliet'? \n",
        "What is the chemical symbol for gold? \n",
        "Who is known as the father of computers? \n",
        "What is the largest planet in our solar system? \n",
        "What is the process by which plants make their food using sunlight called? \n",
        "Who painted the Mona Lisa? \n",
        "What is the name of the first artificial Earth satellite launched by the Soviet Union in 1957? \n",
        "In which year did the Titanic sink? \n",
        "What is the longest river in the world? \n"
    };

    char *answers[QUESTIONS] = {
        "Paris",
        "William Shakespeare",
        "Au",
        "Charles Babbage",
        "Jupiter",
        "Photosynthesis",
        "Leonardo da Vinci",
        "Sputnik 1",
        "1912",
        "The Nile River"
    };

    for (int i = 0; i < QUESTIONS; i++) {
        qn_set[i].question = questions[i];
        qn_set[i].correct_answer = answers[i];
    }
}

int display_and_collect(struct Qn_set qn_set[], struct Answer answer[]) {
    int points_earned = 0;
    for (int i = 0; i < QUESTIONS; i++) {
        printf("%s", qn_set[i].question);
        fgets(qn_set[i].user_answer, ANSWER_LENGTH, stdin);
        qn_set[i].user_answer[strcspn(qn_set[i].user_answer, "\n")] = 0;
        strcpy(answer[i].user_answer, qn_set[i].user_answer);

        if (strcmp(qn_set[i].user_answer, qn_set[i].correct_answer) == 0) {
            points_earned++;
        }
    }
    printf("======================================\n");
    return points_earned;
}

void display_result(int points_earned, struct Qn_set qn_set[], struct Answer answer[]) {
    printf("You have earned %d points out of %d ", points_earned, QUESTIONS);
    for (int i = 0; i < QUESTIONS; i++) {
        printf("%s\n", qn_set[i].question);
        printf("Your answer: %s\n", answer[i].user_answer);
        printf("Correct answer: %s\n\n", qn_set[i].correct_answer);
    }
}

void play_quiz() {
    struct Qn_set qn_set[QUESTIONS];
    struct Answer answer[QUESTIONS];
    int points_earned = 0;

    generate_qn_set(qn_set);
    points_earned = display_and_collect(qn_set, answer);
    display_result(points_earned, qn_set, answer);
}

int main() {
    play_quiz();
    return 0;
}
