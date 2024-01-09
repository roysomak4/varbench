package main

import (
	"bufio"
	"compress/gzip"
	"flag"
	"fmt"
	"os"
	"strings"
)

func main() {
	// parse command line arguments
	queryVcf := flag.String("query-vcf", "", "VCF file to be benchmarked against expected variants. VCF file must be gzipped and tabix indexed.")
	benchmarkVcf := flag.String("benchmark-vcf", "", "VCF file containing benchmark variants. VCF file must be gzipped and tabix indexed.")
	flag.Parse()

	fmt.Println("Loading variants from query VCF...")
	queryVars := loadVcfFile(*queryVcf)
	fmt.Println("Loading variants from benchmark VCF...")
	benchVars := loadVcfFile(*benchmarkVcf)

	outBuff := make([]string, len(queryVars))
	foi = []string {
		"AF",
		"HIAF",
		"DP",
		"HICOV",
		"GENE",
		"HGVS_C",
		"HGVS_P",
	}
	for varSig, bVarInfo := range benchVars {
		outFields := []string
		if qVarInfo, varFound := queryVars[varSig]; varFound {
			outFields = append(outFields, varSig)
			outFields = append(outFields, getInfoValue("AF", qVarInfo))
			outFields = append(outFields, bVarInfo["AF"])
			outFields = append(outFields, getInfoValue("HIAF", qVarInfo))
			outFields = append(outFields, bVarInfo["HIAF"])
			outFields = append(outFields, getInfoValue("DP", qVarInfo))
			outFields = append(outFields, bVarInfo["DP"])
			outFields = append(outFields, getInfoValue("HICOV", qVarInfo))
			outFields = append(outFields, bVarInfo["HICOV"])
			outFields = append(outFields, getInfoValue("HGVS_C", qVarInfo))
			outFields = append(outFields, bVarInfo["HGVS_C"])
			outFields = append(outFields, getInfoValue("HGVS_P", qVarInfo))
			outFields = append(outFields, bVarInfo["HGVS_P"])
			outBuff = append(outBuff, strings.Join(outFields, "\t"))
		} else {
			outFields = append(outFields, varSig)
			outFields = append(outFields, ".")
			outFields = append(outFields, bVarInfo["AF"])
			outFields = append(outFields, ".")
			outFields = append(outFields, bVarInfo["HIAF"])
			outFields = append(outFields, ".")
			outFields = append(outFields, bVarInfo["DP"])
			outFields = append(outFields, ".")
			outFields = append(outFields, bVarInfo["HICOV"])
			outFields = append(outFields, ".")
			outFields = append(outFields, bVarInfo["HGVS_C"])
			outFields = append(outFields, ".")
			outFields = append(outFields, bVarInfo["HGVS_P"])
			outBuff = append(outBuff, strings.Join(outFields, "\t"))
		}


	}
}

func getInfoValue(fieldname string, bVarInfo map[string]string) string {
	retInfo = "."
	if value, keyFound := bVarInfo[fieldname]; keyFound {
		retInfo = value
	}
	return retInfo
}

func loadVcfFile(vcfFile string) {
	variants := make(map[string]map[string]string)

	// open gzipped vcf file
	file, err := os.Open(vcfFile)
	handleError(err, "Error opening VCF file.")
	defer file.Close()

	// Create gzip reader
	reader, err := gzip.NewReader(file)
	handleError(err, "Error handling gzip file.")
	defer reader.Close()

	// create buffered reader to read line by line
	scanner := bufio.NewScanner(reader)
	defer scanner.Close()

	//  read VCF file line by line
	for scanner.Scan() {
		line := scanner.Text()
		if !strings.HasPrefix(line, "#") {
			fields := strings.Split(line, "\t")
			chrom := fields[0]
			pos := fields[1]
			ref := fields[3]
			alt := fields[4]
			variantSignature := chrom + ":" + pos + ":" + ref + ":" + alt
			infoFields := getInfoFields(fields[7])
			variants[variantSignature] = infoFields
		}
	}
	return variants
}

func getInfoFields(info string) map[string]string {
	infoFields := make(map[string]string)

	infoArr := strings.Split(info, ";")
	for _, val := range infoArr {
		infoFieldArr := strings.Split(val, "=")
		infoFields[infoFieldArr[[0]]] = infoFieldArr[1]
	}
	return infoFields
}

func handleError(err error, msg string) {
	if err != nil {
		fmt.Println(msg + " ", err.Error())
		os.Exit(1)
	}
}