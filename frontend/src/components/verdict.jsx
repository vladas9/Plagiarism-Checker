import React, { useState, useEffect, useRef } from 'react';
import Papa from 'papaparse';
import HeatMap from "react-heatmap-grid";
import { InputText } from 'primereact/inputtext';
import Swal from 'sweetalert2';
import FileComparison from './fileComparison';
import withReactContent from 'sweetalert2-react-content'

const MySwal = withReactContent(Swal);

function getMax(a) {
  return Math.max(...a.map(e => Array.isArray(e) ? getMax(e) : e));
}

function getAverage(arr) {
  return arr.flat().reduce((acc, val) => acc + val, 0) / arr.flat().length;
}

function noExtension(filename) {
  var lastDotIndex = filename.lastIndexOf('.');

  if (lastDotIndex === -1) {
    return filename;
  }

  return filename.substring(0, lastDotIndex);
}


export default function UnstyledDemo(props) {
  const data = useRef([]);
  const [grid, setGrid] = useState(null);
  const columnHeaders = useRef([]);
  const rowHeaders = useRef([]);
  const threshold = useRef(50);

  function showFile(value, src, cmp, blocks) {
    return (
      <FileComparison
        plagiarised={value > threshold.current}
        src={src}
        cmp={cmp}
        blocks={blocks} />
    );
  }

  function onGridClick(x, y) {
    const src_file = columnHeaders.current[x];
    const cmp_file = rowHeaders.current[y];
    const val = data.current[x][y];
    const urls = [src_file + ".json", cmp_file + ".json", `${src_file}_vs_${cmp_file}.csv`];
    Promise.all(urls.map(u => fetch(u)))
      .then(res =>
        Promise.all([res[0].json(), res[1].json(), res[2].text()])
      ).then(res => {
        const [src_json, cmp_json, csv] = res;
        console.log("src_json", src_json);
        console.log("cmp_json", cmp_json);
        Papa.parse(csv, {
          download: true,
          delimiter: ',',
          complete: (res, file) => {
            console.log("completed parsing");
            console.log("file", file);
            const lines = file.trim().split('\n');

            const srcBlocks = lines[0].split(',').slice(1);
            console.log("srcBlocks", srcBlocks);
            const cmpBlocks = lines.slice(1).map(line => line.split(',')[0]);
            console.log("cmpBlocks", cmpBlocks);

            const susBlocks = [];
            const matrix = lines.slice(1).map(line => line.split(',').slice(1).map(v => parseFloat(100 - v * 100)));
            console.log("matrix", matrix);
            for (let i = 0; i < matrix.length; i++) {
              for (let j = 0; j < matrix[0].length; j++) {
                console.log("matrix[i][j]", i, j, matrix[i][j]);
                console.log("threshold", threshold);
                if (matrix[i][j] > threshold.current) {
                  console.log("greater than threshold");
                  susBlocks.push([src_json[j], cmp_json[i], matrix[i][j]]);
                }
              }
            }

            console.log("susBlocks", susBlocks);

            const rootElement = document.documentElement
            MySwal.fire({
              title: <p>{`${src_file} & ${cmp_file}`}</p>,
              width: "800px",
              html: showFile(val, src_file, cmp_file, susBlocks),
              color: rootElement.classList.contains("dark") ? "#9CA3AF" : "#545454",
              background: rootElement.classList.contains("dark") ? "#1F2937" : "white",
            })
          }
        })
      });

  }

  useEffect(() => {
    fetch('./all_files.csv')
      .then(res => res.text())
      .then(csv => {
        Papa.parse(csv, {
          download: true,
          delimiter: ',',
          complete: (res, file) => {
            const lines = file.trim().split('\n');

            const srcFiles = lines[0].split(',').slice(1);
            columnHeaders.current = srcFiles;
            console.log("srcFiles", srcFiles);

            const cmpFiles = lines.slice(1).map(line => line.split(',')[0]);
            console.log("cmpFiles", cmpFiles);
            rowHeaders.current = cmpFiles;

            const matrix = lines.slice(1).map(line => line.split(',').slice(1).map(v => parseFloat(100 - v * 100)));
            data.current = matrix;
            console.log("matrix ", data.current);

            setGrid(<div className="flex flex-col justify-items-center gap-12 dark:text-white">
              <HeatMap
                yLabelWidth={60}
                xLabels={srcFiles}
                yLabels={cmpFiles}
                data={matrix}
                cellRender={(value) => value && `${Math.round(value)}%`}
                title={(value, unit) => `${value}`}
                cellStyle={(background, value, min, max, data, x, y) => ({
                  background: `rgba(66, 86, 244, ${1 - (max - value) / (max - min)})`,
                  fontSize: "11px",
                })}
                onClick={(x, y) => { onGridClick(x, y) }}
              />

              <div className="grid grid-cols-3 gap-6 place-content-center place-items-stretch">
                <div className="font-bold bg-white dark:bg-gray-900 rounded-lg shadow-md p-6 transition duration-100 hover:shadow-lg">
                  <div className="text-xl mb-2 dark:text-white">
                    Highest Similarity
                  </div>
                  <span className='text-3xl text-red-900 dark:text-red-200'>
                    {Math.round(getMax(matrix))}%
                  </span>
                </div>

                <div className="font-bold bg-white dark:bg-gray-900 rounded-lg shadow-md p-6 transition duration-100 hover:shadow-lg">
                  <div className="text-xl mb-2 dark:text-white">
                    Average Similarity
                  </div>
                  <span className='text-3xl text-yellow-600 dark:text-yellow-200'>
                    {Math.round(getAverage(matrix))}%
                  </span>
                </div>

                <div className="font-bold bg-white dark:bg-gray-900 rounded-lg shadow-md p-6 transition duration-100 hover:shadow-lg">
                  <div className="text-xl mb-2 dark:text-white">
                    Custom Threashold
                  </div>
                  <div className="mb-4">
                    <InputText keyfilter="int" onChange={(e) => {
                      if (e.target.value.length > 3) return;

                      console.log("setting to ", Number(e.target.value));
                      threshold.current = Number(e.target.value);
                      console.log(threshold);
                    }} />
                  </div>
                </div>

              </div>
            </div>)
          }
        })
      })

  }, []);

  useEffect(() => {
    console.log("set Threshold value: ", threshold.current);
  }, [threshold])

  return (
    <div className="flex flex-col gap-6 bg-white dark:bg-gray-800 rounded-lg shadow-md p-6 text-center">
      <h1 className="text-4xl font-bold tracking-tight text-gray-900 dark:text-white">Verdict</h1>

      <div id="chart">
        {grid ? grid : null}
      </div>
    </div>
  )
}
