#include <stdio.h>
#include <stdlib.h>

struct Ingredient {
    int id;
    float efficacy;
    float mass_mg;
    float efficacy_density;
};

// Comparator for sorting by efficacy density in descending order
int compareIngredients(const void *a, const void *b) {
    const struct Ingredient *ingA = (const struct Ingredient *)a;
    const struct Ingredient *ingB = (const struct Ingredient *)b;

    if (ingB->efficacy_density > ingA->efficacy_density)
        return 1;
    if (ingB->efficacy_density < ingA->efficacy_density)
        return -1;
    return 0;
}

// Clear invalid input buffer
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int n, i;
    float safety_limit, total_efficacy = 0;

    printf("==================================================\n");
    printf("   PHARMACEUTICAL DRUG FORMULATION OPTIMIZER\n");
    printf("==================================================\n");

    printf("Enter the number of available active ingredients: ");
    while (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input. Enter a positive integer: ");
        clearBuffer();
    }

    struct Ingredient *ingredients = (struct Ingredient *)malloc(n * sizeof(struct Ingredient));

    if (ingredients == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Input ingredients
    for (i = 0; i < n; i++) {
        ingredients[i].id = i + 1;

        printf("\n--- Ingredient %d ---\n", ingredients[i].id);

        printf("Enter Therapeutic Efficacy Score: ");
        while (scanf("%f", &ingredients[i].efficacy) != 1 || ingredients[i].efficacy < 0) {
            printf("Invalid input. Enter non-negative value: ");
            clearBuffer();
        }

        printf("Enter Mass (mg): ");
        while (scanf("%f", &ingredients[i].mass_mg) != 1 || ingredients[i].mass_mg <= 0) {
            printf("Invalid input. Enter value greater than 0: ");
            clearBuffer();
        }

        ingredients[i].efficacy_density =
            ingredients[i].efficacy / ingredients[i].mass_mg;
    }

    printf("\nEnter Maximum Safe Formulation Limit (mg): ");
    while (scanf("%f", &safety_limit) != 1 || safety_limit < 0) {
        printf("Invalid input. Enter non-negative value: ");
        clearBuffer();
    }

    // Sort ingredients
    qsort(ingredients, n, sizeof(struct Ingredient), compareIngredients);

    printf("\n==================================================\n");
    printf("FORMULATION REPORT\n");
    printf("==================================================\n");

    // Fractional Knapsack Logic
    for (i = 0; i < n; i++) {
        if (safety_limit <= 0)
            break;

        if (safety_limit >= ingredients[i].mass_mg) {
            total_efficacy += ingredients[i].efficacy;
            safety_limit -= ingredients[i].mass_mg;

            printf("Included Ingredient %d: 100%% (%.2f mg added)\n",
                   ingredients[i].id,
                   ingredients[i].mass_mg);
        } else {
            float fraction = safety_limit / ingredients[i].mass_mg;
            float partial_efficacy =
                ingredients[i].efficacy_density * safety_limit;

            total_efficacy += partial_efficacy;

            printf("Included Ingredient %d: %.2f%% (%.2f mg added)\n",
                   ingredients[i].id,
                   fraction * 100,
                   safety_limit);

            safety_limit = 0;
        }
    }

    printf("\nTotal Therapeutic Efficacy Achieved: %.2f\n", total_efficacy);

    free(ingredients);

    return 0;
}
