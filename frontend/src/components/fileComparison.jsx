import CodeBlock from "./codeBlock";

const red = <span className="bg-red-100 text-red-800 text-xl font-medium me-2 px-2.5 py-0.5 rounded-full dark:bg-red-900 dark:text-red-300">Plagiarised</span>;
const green = <span className="bg-green-100 text-green-800 text-xl font-medium me-2 px-2.5 py-0.5 rounded-full dark:bg-green-900 dark:text-green-300">Not Plagiarised</span>

const code = `
// Sample code block
function hi() {
    console.log("Hello, World!");
}
hi();`

export default function FileComparison({ plagiarised, src, cmp, blocks }) {
  console.log(blocks);
  return (
    <div className="rounded-lg shadow-md p-6 transition duration-100 hover:shadow-lg text-center">
      <h1 className="mb-4 text-4xl font-bold tracking-tight text-gray-900 dark:text-white">Verdict</h1>
      <p className="font-normal text-xl text-gray-700 dark:text-gray-400"> {"The code is "} {plagiarised ? red : green}</p>
      {blocks.length > 0
        ? <p className="pb-4 pt-8 font-normal text-xl text-gray-700 dark:text-gray-400">Code blocks that look similar: </p>
        : null
      }
      <div className="flex flex-col gap-6">
        {blocks.map(b =>
          <div className="flex flex-col justify-center gap-4">
            <p className="font-normal text-xl text-gray-700 dark:text-gray-400">{`Similarity: ${Math.round(b[2])}%`}</p>
            <CodeBlock path={src} code={b[0]} />
            <CodeBlock path={cmp} code={b[1]} />
          </div>)}
      </div>
    </div>
  )
}
