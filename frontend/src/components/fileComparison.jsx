import CodeBlock from "./codeBlock";

const red = <span className="bg-red-100 text-red-800 text-xl font-medium me-2 px-2.5 py-0.5 rounded-full dark:bg-red-900 dark:text-red-300">Plagiarised</span>;
const green = <span className="bg-green-100 text-green-800 text-xl font-medium me-2 px-2.5 py-0.5 rounded-full dark:bg-green-900 dark:text-green-300">Not Plagiarised</span>

const code = `
// Sample code block
function hi() {
    console.log("Hello, World!");
}
hi();`

export default function FileComparison({ plagiarised }) {
    console.log(plagiarised);
    return (
        <div className="card text-center">
            <h1 className="mb-4 text-4xl font-bold tracking-tight text-gray-900 dark:text-white">Verdict</h1>
            <p className="font-normal text-xl text-gray-700 dark:text-gray-400"> {"The code is "} {plagiarised ? red : green}</p>

            <p className="pt-8 font-normal text-xl text-gray-700 dark:text-gray-400">Code blocks that look similar: </p>

            <CodeBlock path="/path/to/original/file" code={code}/>
        </div>
    )
}