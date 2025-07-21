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

// Array methods with C++ objects
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

// Async/await with C++ promise integration
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

// Demo execution
console.log("\nRunning Complete Demo:");

async function runDemo(): Promise<void> {
    // Test error handling
    await testErrorHandling();

    // Demonstrate promise integration
    console.log("\nSimple brew demonstration:");
    await brewCoffee(espresso);

    console.log("\nDemo completed! TypeScript + V8 provides seamless C++ integration.");
}

// Execute the demo
runDemo().catch(error => {
    console.log("Demo failed:", error);
});
