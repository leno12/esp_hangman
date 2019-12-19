import json
import diff_match_patch as dmp_lib
import xml.etree.ElementTree as ET
import yaml 


def get_pretty_diff(jsstring):
    dmp = dmp_lib.diff_match_patch()
    d = json.loads(jsstring)
    return dmp.diff_prettyHtml(d)


def generateHTMLReport(results,description = None):
    root = ET.Element("html")
    header = ET.SubElement(root,"head")
    style = ET.SubElement(header,"style")
    style.text = \
    """
    
    """
    w3css = ET.SubElement(header,"link")
    w3css.attrib["rel"] = "stylesheet"
    w3css.attrib["href"]= "https://www.w3schools.com/w3css/4/w3.css"
    icons = ET.SubElement(header,"link")

    icons.attrib["rel"] = "stylesheet"
    icons.attrib["href"]= "https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css"
    meta = ET.SubElement(header,"meta")
    meta.attrib["charset"] = "utf-8"
    body = ET.SubElement(root,"body")
    generateSummary(body,results,description)
    return  "<!DOCTYPE html>\n"+ET.tostring(root,encoding="utf-8").decode("utf-8")
    

def generateSummary(p,results,definition):
    c = ET.SubElement(p,"div")
    log = results.pop("log")
    if log["builds"] == True:
        capt = ET.SubElement(c,"h2")
        capt.text = "Summary of testcase results"
        t = ET.SubElement(c,"table")
        t.attrib["class"]="w3-table-all w3-card-4"
        tr = ET.SubElement(t,"tr")
        desc = ET.SubElement(t,"th")
        desc.text = "Name"
        desc = ET.SubElement(t,"th")
        desc.text = "Status"
        desc = ET.SubElement(t,"th")
        desc.text = "Score"
        desc = ET.SubElement(t,"th")
        desc.text = "Valgrind Errors"
        desc = ET.SubElement(t,"th")
        desc.text = "Valgrind Contexts"

        for name,tc_result in results.items():
            tr = ET.SubElement(t,"tr")
            td = ET.SubElement(tr,"td")
            link_to_tc = ET.SubElement(td,"a")
            link_to_tc.text = name
            link_to_tc.attrib["href"] = "#{}".format(name)
            td = ET.SubElement(tr,"td")
            if tc_result["implemented"]==False:
                td.text = "not implemented"
                td.attrib["class"]="w3-orange"
                i = ET.SubElement(td,"i")
                i.attrib["class"] = "fa fa-file"
            elif tc_result["passed"]==False:
                td.text = "failed"
                td.attrib["class"]="w3-red"
                i = ET.SubElement(td,"i")
                i.attrib["class"] = "fa fa-bug"
            else:
                td.text = "passed"
                td.attrib["class"]="w3-green"
                i = ET.SubElement(td,"i")
                i.attrib["class"] = "fa fa-check"
                pass
            td = ET.SubElement(tr,"td")
            v = str(int(100*tc_result["score"]))
            prog = ET.SubElement(td,"div")
            prog.attrib["class"] = "w3-light-grey w3-round"
            prog2 = ET.SubElement(prog,"div")
            prog2.attrib["class"] = "w3-container w3-round w3-blue"
            prog2.attrib["style"] = "width:{}%".format(v)
            prog2.text = "{}%".format(v)

            td = ET.SubElement(tr,"td")
            td.text = str(tc_result["valgrind"]["errors"])
            td.attrib["width"] = "5%"
            td.attrib["style"] = "text-align:center"
            td = ET.SubElement(tr,"td")
            td.text = str(tc_result["valgrind"]["contexts"])
            td.attrib["style"] = "text-align:center"
            td.attrib["width"] = "5%"

        det_report = ET.SubElement(c,"div")
        tc_def = None
        if definition is not None:
            tc_def = definition["testcases"]
        for name,tc_result in results.items():
            #get corresponding description
            d = list(filter(lambda x: x["name"]==name,tc_def))[0]
            detail_report(tc_result,name,det_report,d)
        

    else:
        no_build = ET.SubElement(c,"div")
        no_build.attrib["class"] = "w3-panel w3-yellow"
        h3 = ET.SubElement(no_build,"h3")
        h3.text = "Your assignment does not compile, please fix your code!"


def detail_report(tc,name,parent,tc_def = None):
    card = ET.SubElement(parent,"div")
    card.attrib["id"]=name
    card.attrib["class"] = "w3-card"
    #i = ET.SubElement(card,"i")
    #i.attrib["class"] = "fa fa-check"
    title = ET.SubElement(card,"h2")
    title.text = name

    if tc_def is not None:
        desc_panel = ET.SubElement(card,"div")
        desc_panel.attrib["class"] = "w3-panel w3-pale-green w3-serif"
        desc = ET.SubElement(desc_panel,"p")
        desc.text = tc_def["description"]
    output_diff = ET.SubElement(card,"div")
    dmp = dmp_lib.diff_match_patch()
   
    for diff in tc["diff"]:
        jdiff = json.loads(diff)
        p = ET.SubElement(output_diff,"div")
        for diffline in jdiff:
            s=dmp.diff_prettyHtml([diffline]).replace("<br>","<br/>").replace("&para;","")
            print(s)
            op = ET.fromstring(s)
            p.append(op)

       
        #for diffline in diff:
            #op = ET.fromstring(get_pretty_diff(diffline))
            #print(op)
with open("test_definition/testdef.yml") as f:
    tc_def = yaml.load(f,Loader=yaml.FullLoader)

with open("results/results.json") as f:
    results = json.load(f)
    report = generateHTMLReport(results,description = tc_def)
    print(report)
with open("results/report.html","w") as f:
    f.write(report)
