import React, { useState, useEffect, useRef } from 'react';
import Papa from 'papaparse';
import HeatMap from "react-heatmap-grid";
import { InputText } from 'primereact/inputtext';
import Swal from 'sweetalert2';
import FileComparison from './fileComparison';
import withReactContent from 'sweetalert2-react-content'

const MySwal = withReactContent(Swal);

function getMax(a){
    return Math.max(...a.map(e => Array.isArray(e) ? getMax(e) : e));
}

function getAverage(arr) { 
    return arr.flat().reduce((acc, val) => acc + val, 0) / arr.flat().length;
}


export default function UnstyledDemo(props) {    
    const data = useRef([]);
    const [grid, setGrid] = useState(null);
    const columnHeaders = useRef([]);
    const rowHeaders = useRef([]);
    const threshold = useRef(50);

    function showFile(value) {
        console.log(threshold.current);
        return (<FileComparison plagiarised={value > threshold.current}/>);
    }

    function onGridClick(x, y)  {
        const val = data.current[x][y];
        MySwal.fire({
            title: <p>Hello World</p>,
            html: showFile(val)
        })
    }
  
    useEffect(() => {  
      fetch('./general.csv')
      .then( res => res.text() )
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

                const matrix = lines.slice(1).map(line => line.split(',').slice(1).map(v => parseFloat(v * 100)));
                data.current = matrix;
                console.log("matrix ", data.current);

                // const result = [];
                // for (let i = 0; i < srcFiles.length; i++) {
                //     for (let j = 0; j < cmpFiles.length; j++) {
                //         result.push({
                //             srcfile: srcFiles[i].replace('\r', ''),
                //             cmpfile: cmpFiles[j],
                //             value: matrix[i][j]
                //         });
                //     }
                // }
                // console.log(result);

                setGrid(<div className="dark:text-white">
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
                        onClick={(x, y) => { onGridClick(x, y)} }  
                    />

                    <div className="grid items-center mt-4 ml-8 center grid-cols-3">
                        <div className="max-w-sm mt-4 rounded overflow-hidden shadow-lg">
                            <div className="px-6 py-4">
                                <div className="font-bold text-xl mb-2 dark:text-white">Highest Similarity</div>
                                <span className="text-3xl text-red-800 font-bold text-gray-900 dark:text-white">{Math.round(getMax(matrix))}%</span>
                            </div>
                        </div>
                        <div className="max-w-sm mt-4 rounded overflow-hidden shadow-lg">
                            <div className="px-6 py-4">
                                <div className="font-bold text-xl mb-2 dark:text-white">Average Similarity</div>
                                <span className="text-3xl text-yellow-500 font-bold text-gray-900 dark:text-white">{Math.round(getAverage(matrix))}%</span>
                            </div>
                        </div>
                        <div className="max-w-sm mt-4 rounded overflow-hidden shadow-lg">
                            <div className="font-bold text-xl mb-2  dark:text-white">Plagiarism Threshold</div>
                            <div className="mb-4">
                                <InputText keyfilter="int" onChange={(e) => {
                                    if(e.target.value.length > 3) return;

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
        console.log("set Threshold value: ",  threshold.current);
    }, [threshold])
      
    return (
        <div className="card text-center">
            <h1 className="mb-4 text-4xl font-bold tracking-tight text-gray-900 dark:text-white">Verdict</h1>

            <div id="chart" className="mt-4">
                {grid ? grid : null}
            </div>            
        </div>
    )
}