// Auto-generated TypeScript definitions for V8 bindings

/**
 * Waits for the specified number of milliseconds.
 * @param milliseconds The number of milliseconds to wait
 * @returns A promise that resolves after the specified delay
 */
declare function wait(milliseconds: number): Promise<void>;

/**
 * Represents a coffee machine that can brew recipes.
 */
declare class CoffeeMachine {
    /**
     * Creates a new coffee machine instance.
     * @param name The name of the coffee machine
     */
    constructor(name: string);

    /**
     * Turns on the coffee machine.
     */
    turnOn(): void;

    /**
     * Turns off the coffee machine.
     */
    turnOff(): void;

    /**
     * Brews coffee using the specified recipe.
     * @param recipe The recipe to brew
     * @returns A promise that resolves with a success message when brewing is complete
     */
    brew(recipe: Recipe): Promise<string>;

    /**
     * Gets the name of the coffee machine.
     * @returns The machine name
     */
    getName(): string;
}

/**
 * Represents a coffee recipe with brewing parameters.
 */
declare class Recipe {
    /**
     * Creates a new recipe.
     * @param name The name of the recipe
     * @param strength The coffee strength (0-100)
     * @param waterAmount The amount of water in milliliters
     * @param brewTime The brewing time in milliseconds
     */
    constructor(name: string, strength: number, waterAmount: number, brewTime: number);

    /**
     * Gets the recipe name.
     * @returns The recipe name
     */
    getName(): string;

    /**
     * Gets the coffee strength.
     * @returns The strength percentage (0-100)
     */
    getStrength(): number;

    /**
     * Gets the brewing time.
     * @returns The brewing time in milliseconds
     */
    getBrewTime(): number;

    /**
     * Gets a formatted description of the recipe.
     * @returns A string describing the recipe parameters
     */
    getDescription(): string;
}

/**
 * Console object for logging.
 */
declare const console: {
    /**
     * Logs messages to the console.
     * @param args The values to log
     */
    log(...args: any[]): void;
};
