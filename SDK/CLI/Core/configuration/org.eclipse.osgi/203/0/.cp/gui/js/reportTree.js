/*
##############################################################################################################################
##                                                                                                                          ##
##                                              custom.js File                                                              ##
##                                                                                                                          ##
##############################################################################################################################
 */

$(document).ready(function() {
    $("body").on("click", ".node", function(e) {
        processNodes(this)
    });
});

function processNodes(elem) {
    if ($(elem).hasClass("rootNode")) {
        $("." + $(".selectedFilter").attr("filtervalue")).parent().show();
        return;
    }

    let parentDiv = $(elem).parent()

    // Loop through direct children
    $(parentDiv).children().each(function() {

        // Only <div> children matter for show or hide
        if ($(this)[0].nodeName == "DIV" && ($(this)[0].className.includes("node-show") || $(this)[0].className.includes("node-hide")) ) {
            if ($(this)[0].className.includes("node-hide")) {
                $(this).attr("class", "indent-node node-show")
            } else {
                $(this).attr("class", "indent-node node-hide")
            }
        }
    });
}

function checkExpandNodes(expandNodes) {
    if (expandNodes == "expand") {
        $(".node-hide").attr("class", "indent-node node-show")
        return;
    }
    if (expandNodes == "fold") {
        $(".node-show").attr("class", "indent-node node-hide")
    }
}

function filterNodes(filterBtn) {
    if (filterBtn.getAttribute("filterValue") == "rootObj" && !filterBtn.className.includes("selectedFilter")) {
        $(".generator").remove();
        createScopeValidationNodes($('#validation-result'))
        createScopeGenerationNodes($('#generation-result'))
    }
    if (filterBtn.getAttribute("filterValue") == "generator" && !filterBtn.className.includes("selectedFilter")) {
        $(".rootObj").remove();
        createListOfResults(getValidationData(), $('#validation-result'), 'generator')
        createGeneratorNodes(data.generationProcessResult.generationResult, $('#generation-result'), 'generator')
    }

    $(".filterGroup").children().removeClass("selectedFilter")
    $(filterBtn).addClass("selectedFilter")

}

// Validation results
    // Add title
    $('#title').append(data.name)

    //Validation result header
    function addErrorCount(text, value) {
        return `<div><div class="elem bold fix-width inline-block"><li class="s">${text}</li></div><div class="elem inline">${value}</div></div>`
    }

    $('#validation-header').append(addErrorCount('Fatal Errors', ((data.validationResults?.fatal_errors) ? data.validationResults.fatal_errors : 0)))
    $('#validation-header').append(addErrorCount('Errors', ((data.validationResults?.errors) ? data.validationResults.errors : 0)))
    $('#validation-header').append(addErrorCount('Warnings', ((data.validationResults?.warnings) ? data.validationResults.warnings : 0)))
    $('#validation-header').append(addErrorCount('Improvements', ((data.validationResults?.improvements) ? data.validationResults.improvements : 0)))
    $('#validation-header').append(addErrorCount('Infos', ((data.validationResults?.infos) ? data.validationResults.infos : 0)))

    //Validation result data
    function createScopeValidationNodes(parentNode) {
        let allData = getValidationData()
        for(const [name, results] of Object.entries(groupBy(allData, ({scope}) => scope.contextObject))) {
            const scopeName = name.slice(Math.max(name.lastIndexOf("/") + 1 ,0))
            const scopeType = results[0].scope.contextObjectType

            const scopeNode = $('<div/>', {class: 'indent-node rootObj'})
            scopeNode.append(`
                <span class="pointer node space-to-icon close-node-icon node-width">
                    <span class="icon-${scopeType} elem">${scopeName}</span>
                </span>`)
            createListOfResults(results, scopeNode, 'node-hide')
            parentNode?.append(scopeNode)
        }
    }

    //Creates a list of sub divs for each value
    function createListOfResults(results, parentNode, classes) {
        for(const [id, values] of Object.entries(groupBy(results, (valRes) => valRes?.validationId))) {
                let valResult = values[0];
                const isOndemand = valResult.onDemandResult ? "-OnDemand" : ""
                const severity = valResult.severity
                const validationResultGropeNode = createNode(`${valResult.validationId} (${valResult.message})`, `icon${isOndemand}-${severity}`, true, classes)

                values.forEach(val => {
                    const isOndemand = val.onDemandResult ? "-OnDemand" : ""
                    const severity = val.severity
                    validationResultGropeNode?.append(createNode(val.description, `icon${isOndemand}-${severity}`, false))
                })

                parentNode?.append(validationResultGropeNode)
        }
    }

    function getValidationData() {
        const validationResultId = data.validationResults?.validationResultId;
        if (!Array.isArray(validationResultId)) {
            return [];
        }

        return [].concat(...(mapValidationIdToResult(validationResultId).map(element => unpackScopes(element))));
    }

    function mapValidationIdToResult(values) {
        return [].concat(...values.map(valId => createMappedElement(valId?.validationResult, valId?.origin, valId?.id, valId?.message)));
    }

    function createMappedElement(results, origin, id, message) {
        return results.map(res => {
            res.validationId = origin + id;
            res.message = message
            return res
        })
    }

    function unpackScopes(el) {
        let result = []
        if (el.scopes.scope == null)  {
            result.push ({
                message: el.message,
                description: el.description,
                onDemandResult: el.ondemandresult,
                severity:  el.severity,
                validationId: el.validationId,
                scope: {
                    contextObject:  "Non Scoped Validation Result" ,
                    contextObjectType:  "file"
                }
            })
            return result
        }
        el.scopes.scope.forEach(scope => {
            result.push ({
                message: el.message,
                description: el.description,
                onDemandResult: el.ondemandresult,
                severity:  el.severity,
                validationId: el.validationId,
                scope: {
                    contextObject: scope.contextObject == null ? "" : scope.contextObject.slice(Math.max(scope.contextObject.lastIndexOf("/") + 1 ,0)),
                    contextObjectType: scope.contextObjectType == null ? "Non scoped results" : scope.contextObjectType
                }
            })
        })
        return result
    }

    $(document).ready(() => createScopeValidationNodes($('#validation-result')))

    //Generation results
    $('#generation-header').append(addErrorCount('Execution Result', data.generationProcessResult.result))
    $('#generation-header').append(addErrorCount('Last execution time', data.generationProcessResult.lastexecutiontime))
    $('#generation-header').append(addErrorCount('Last duration', data.generationProcessResult.lastexecutionduration))


    function createScopeGenerationNodes(parentNode) {
        for(const [name, results] of Object.entries(groupBy(data.generationProcessResult.generationResult, ({scope}) => scope.contextObject))) {
            const scopeType = results[0].scope.contextObjectType == null ? "file" : results[0].scope.contextObjectType
            const scopeName = name == 'undefined' ? "Non Scoped Generator" : name.slice(Math.max(name.lastIndexOf("/") + 1 ,0))
            const scopeNode = $('<div/>', {class: 'indent-node rootObj'})
            scopeNode.append(`
                <span class="pointer node space-to-icon close-node-icon node-width scopeNode">
                    <span class="icon-${scopeType} elem">${scopeName}</span>
                </span>`)

            createGeneratorNodes(results, scopeNode, 'node-hide')
            //Add scope nodes to the DOM
            parentNode?.append(scopeNode)
        }
    }

    function createGeneratorNodes(generationResults, parentNode, classes) {
        for(const genRes of generationResults) {
            const genResult = genRes.state.toLowerCase()
            const genType = genRes.generationtype == "GENERATOR" ? "internal" : "external"
            const generatorNode = createNode(genRes.name, `icon-genres-${genType}-${genResult} space-to-icon-extended`, true, classes)

            const phaseNode = createNode('Phases', 'icon-phases', true)
            genRes.phases.phase.forEach(phase => {
                //TODO check if this should also be done for ConsolOutput, and messages.
                const hasChildren = phase.generatedFiles?.file && phase.generatedFiles.file.length > 0

                const phaseNodeDiv = createNode(phase.type, `icon-state-${phase.state.toLowerCase()}`, hasChildren )

                if (phase.generatedFiles?.file && phase.generatedFiles.file.length > 0) {
                    const generatedFilesNode = createNode('Generated Files', 'icon-file-set', true)
                    phaseNodeDiv.append(generatedFilesNode)
                    phase.generatedFiles.file.forEach(file => {
                        generatedFilesNode.append(createNode(`${file.value} [<b>${file.generationinfo}</b>]`, 'icon-file'), false)
                    })
                }
                phaseNode.append(phaseNodeDiv)
            })

            const generatorInfoNode = createNode(genRes.generator.name, 'icon-generation', true)
            generatorInfoNode.append(
                createNode(`Version: ${genRes.generator.version}`, 'icon-file', false),
                createNode(`MSN: ${genRes.generator.msn}`, 'icon-file', false),
                createNode(`Definition: ${genRes.generator.definition}`, 'icon-file', false))
            generatorNode.append(phaseNode, generatorInfoNode)
            if (genRes.scope.contextObject != null) {
                const scopetype = genRes.scope.contextObjectType
                const scopePath = genRes.scope.contextObject
                const scopeName = scopePath.slice(Math.max(scopePath.lastIndexOf("/") + 1 ,0))
                const scopeInfoNode = createNode(scopeName, `icon-${scopetype}`, true)
                scopeInfoNode.append(createNode(`Root Object: ${scopePath}`, `icon-${scopetype}`, false))

                generatorNode.append(scopeInfoNode)
            }

            parentNode.append(generatorNode)
        }
    }

    function createNode(label, icon, collapse, classes = 'node-hide') {
        const pointer = collapse ? "pointer" : "cursor"
        return $(`<div/>`, {class: `indent-node ${classes}`})
            .append(`<span class="${pointer} node node-width space-to-icon ${collapse ? "close-node-icon" : "" }"><span class="${icon} elem">${label}</span></span>`)
    }

    function groupBy(array, keyGetter) {
        const map = new Map();
        array.forEach((item) => {
            const key = keyGetter(item);
            const collection = map.get(key);
            if (!collection) {
                map.set(key, [item]);
            } else {
                collection.push(item);
            }
        });
        return Object.fromEntries(map);
    }

    $(document).ready(() => createScopeGenerationNodes($('#generation-result')))
