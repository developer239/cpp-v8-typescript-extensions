/// <reference path="./types.d.ts" />

console.log("V8 TypeScript Demo - Coffee Brewing Simulation\n");

// Define coffee recipes with varying parameters
console.log("Creating Coffee Recipes:");
const espresso = new Recipe("Espresso", 100, 30, 2000);
const americano = new Recipe("Americano", 80, 150, 3000);
const latte = new Recipe("Latte", 70, 200, 4000);
const customBlend = new Recipe("Morning Special", 85, 180, 3500);

console.log(espresso.getDescription());
console.log(americano.getDescription());
console.log(latte.getDescription());
console.log(customBlend.getDescription());

// Initialize coffee machine
console.log("\nSetting up Coffee Machine:");
const coffeeMachine = new CoffeeMachine("Professional Barista 3000");
console.log(`${coffeeMachine.getName()} is ready`);

// Demonstrate JavaScript array methods with C++ objects
console.log("\nJavaScript Array Methods with C++ Objects:");

const recipes = [espresso, americano, latte, customBlend];

// Filter recipes by strength
const strongRecipes = recipes.filter(r => r.getStrength() > 75);
console.log(`Strong recipes (>75%): ${strongRecipes.map(r => r.getName()).join(", ")}`);

// Transform recipe data
const recipeStats = recipes.map(recipe => ({
    name: recipe.getName(),
    strength: recipe.getStrength(),
    time: recipe.getBrewTime() / 1000 + "s"
}));

console.log("\nRecipe statistics:");
recipeStats.forEach(({ name, strength, time }) => {
    console.log(`  ${name}: ${strength}% strength, ${time} brew time`);
});

// Demonstrate async/await with C++ promise integration
console.log("\nAsync Coffee Brewing:");

async function brewCoffee(recipe: Recipe): Promise<void> {
    console.log(`\nPreparing ${recipe.getName()}...`);

    // Turn on the machine
    coffeeMachine.turnOn();
    console.log("Machine turned on");

    // Wait for machine to warm up
    await wait(500);
    console.log("Machine ready");

    // Brew the coffee and measure time
    try {
        console.log(`Brewing ${recipe.getName()} (${recipe.getBrewTime()}ms)...`);
        const startTime = Date.now();
        const result = await coffeeMachine.brew(recipe);
        const actualBrewTime = Date.now() - startTime;
        console.log(result);
        console.log(`Brew time: ${actualBrewTime}ms`);
    } catch (error) {
        console.log(`Brewing failed: ${error}`);
    }

    // Turn off the machine
    coffeeMachine.turnOff();
    console.log("Machine turned off");
}

// Test error handling
console.log("\nError Handling:");

async function testErrorHandling(): Promise<void> {
    try {
        // Attempt to brew without turning on the machine
        const testMachine = new CoffeeMachine("Test Machine");
        await testMachine.brew(espresso);
    } catch (error) {
        console.log(`Error correctly caught: "${error}"`);
    }

    try {
        // Attempt to brew without a recipe
        coffeeMachine.turnOn();
        await coffeeMachine.brew(null as any);
    } catch (error) {
        console.log(`Error correctly caught: "${error}"`);
    }
}

// Coffee shop simulation using OOP
console.log("\nCoffee Shop Simulation:");

class CoffeeShop {
    private machine: CoffeeMachine;
    private orders: Array<{ recipe: Recipe, time: number }> = [];

    constructor(machine: CoffeeMachine) {
        this.machine = machine;
    }

    async serveCustomer(recipe: Recipe): Promise<number> {
        const startTime = Date.now();
        await brewCoffee(recipe);
        const totalTime = Date.now() - startTime;

        this.orders.push({ recipe, time: totalTime });
        return totalTime;
    }

    getStats() {
        const totalOrders = this.orders.length;
        const avgTime = this.orders.reduce((sum, order) => sum + order.time, 0) / totalOrders || 0;

        const popularRecipe = this.orders.reduce((acc, order) => {
            acc[order.recipe.getName()] = (acc[order.recipe.getName()] || 0) + 1;
            return acc;
        }, {} as Record<string, number>);

        return {
            totalOrders,
            averageTime: Math.round(avgTime) + "ms",
            popularRecipes: popularRecipe
        };
    }
}

// Advanced async patterns
console.log("\nAdvanced Async Patterns:");

async function morningRush(): Promise<void> {
    console.log("\nMorning rush starting...");

    const shop = new CoffeeShop(coffeeMachine);

    // Define customer orders
    const orders = [
        { customer: "Alice", recipe: espresso },
        { customer: "Bob", recipe: latte },
        { customer: "Charlie", recipe: americano }
    ];

    // Process orders sequentially (single coffee machine constraint)
    for (const { customer, recipe } of orders) {
        console.log(`\nOrder for ${customer}: ${recipe.getName()}`);
        const time = await shop.serveCustomer(recipe);
        console.log(`${customer}'s order completed in ${time}ms`);
        await wait(1000); // Brief pause between orders
    }

    // Display statistics
    console.log("\nMorning Rush Statistics:");
    console.log(shop.getStats());
}

// Main demo execution
console.log("\nRunning Complete Demo:");

async function runDemo(): Promise<void> {
    // Test error handling
    await testErrorHandling();

    // Demonstrate promise integration
    console.log("\nSimple brew demonstration:");
    await brewCoffee(espresso);

    // Run coffee shop simulation
    await morningRush();

    console.log("\nDemo completed! TypeScript + V8 provides seamless C++ integration.");
}

// Execute the demo
runDemo().catch(error => {
    console.error("Demo failed:", error);
});
