function [fig] = plotLog_sousvide(filename,varargin)

numFields=5;

[~,name,~]=fileparts(filename);
if nargin>1
    customTitle=varargin{1};
else
    customTitle=string(regexprep(name,'_','\\_'));  % Escape "_" for latex interpreter
end

formatstr='';
for n=1:numFields
    formatstr=strcat(formatstr,"%c%f ");
end

tab=readtable(filename,'Format',formatstr,...
    'Delimiter','comma','HeaderLines',0,'ReadVariableNames',false);

if numel(tab.Properties.VariableNames)/2 ~= numFields
    error(['The number of fields specified in "numFields" (%d) does not'...
        ' match the number of columns in the log file (%d)'],...
        numFields,tab.Properties.VariableNames/2);
end

for var=2:2:numFields*2   % Assign names to columns
    tab.Properties.VariableNames{var}=tab{1,var-1};
end

tab(:,1:2:numel(tab.Properties.VariableNames))=[]; % Remove odd columns

tab.t=tab.t/1000;   % Convert ms to s
tab.T=tab.T/100;    % Convert mºC to ºC
tab.r=tab.r/100;    % Convert mºC to ºC

set(0,'DefaultTextInterpreter','latex');
fig=figure(); hold on;
    plotModes(tab);
    % Colors obtained with get(groot,'DefaultAxesColorOrder')
    plt_T=plot(tab.t,tab.T,'-','Color',[0 0.4470 0.7410]);
    plt_t=plot(tab.t,tab.r,'-','Color',[0.8500 0.3250 0.0980]);
    plt_r=plot(tab.t,tab.o,'-','Color',[0.9290 0.6940 0.1250]);
    legend([plt_T,plt_t,plt_r],"Measured","Reference","Output",'Location','southeast');
    xlabel("t [s]"); ylabel("T [$^\circ$C]");
    axis tight;
    grid on;
    title(customTitle);

if nargin>=3 && varargin{2}
    print(name+".eps",'-depsc');
end

end

function [] = plotModes(tab)

    % Mode codes
    M_HYSTERESIS=0;
    M_PI_WEIGHT =1;
    M_PI_WINDUP =2;
    M_PI_FILTER =3;
    M_EVENTS    =4;

    row=1;
    mode=tab{row,1};
    start_row=row;
    while row<height(tab)   % Scan the whole table
        row=row+1;
        if tab{row,1}~=mode || row==height(tab) % If next row changes mode
            x=[tab.t(start_row),tab.t(start_row),tab.t(row),tab.t(row)];
            y=[10,0,0,10];
            alpha=0.5;    % Default
            switch mode
                case M_HYSTERESIS
                    color=[0.6350 0.0780 0.1840];
                case M_PI_WEIGHT
                    color=[0.3010 0.7450 0.9330];
                case M_PI_WINDUP
                    color=[0.4660 0.6740 0.1880];
                case M_PI_FILTER
                    color=[0.4940 0.1840 0.5560];
                case M_EVENTS
                    color=[0 1 1]; % Yellow
                otherwise
                    color=[1 1 1];  % White
                    alpha=0;        % Transparent
            end
            fill(x,y,color,'FaceAlpha',alpha,'EdgeColor','none');
            start_row=row;
        end
        mode=tab{row,1};
    end

end
